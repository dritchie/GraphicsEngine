#include "Assets/Texture/Texture.h"
#include "Common/GL.h"
#include "Utility/Timer.h"
#include "Math/Math.h"
#include "Assets/Shader/ShaderProgram.h"
#include "FreeImage.h"
#include <map>
#include <unordered_map>

using namespace std;

namespace GraphicsEngine
{
	/**
	Hidden utility class for caching texture-->texture unit assignments
	**/
	class TexUnitCache
	{
	public:

		TexUnitCache();
		UINT GetTextureUnit(Texture* tex);
		void RemoveFromCache(Texture* tex);

	private:

		void BindTexture(UINT unit, Texture* texture);

		struct TexRecord
		{
			UINT unit;
			double lastTouched;
		};

		Timer timer;
		UINT smallestFreeUnit;
		map<UINT, Texture*> unit2tex;
		unordered_map<Texture*, TexRecord> tex2rec;
	};

	TexUnitCache::TexUnitCache()
		: smallestFreeUnit(0)
	{
		timer.start();
	}

	void TexUnitCache::BindTexture(UINT unit, Texture* texture)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(texture->texType, texture->texName);
	}

	UINT TexUnitCache::GetTextureUnit(Texture* tex)
	{
		double touch = timer.getElapsedTimeInMilliSec();

		// CASE 1: The texture is in cache
		auto recit = tex2rec.find(tex);
		if (recit != tex2rec.end())
		{
			recit->second.lastTouched = touch;
			return recit->second.unit;
		}
		// CASE 2: The texture is not in cache, but
		// the cache is not full
		else if (unit2tex.size() < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
		{
			unit2tex[smallestFreeUnit] = tex;
			TexRecord& tr = tex2rec[tex];
			tr.unit = smallestFreeUnit;
			tr.lastTouched = touch;

			// Don't forget to store the next free unit (if the cache is not full)
			smallestFreeUnit++;
			if (unit2tex.size() < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
			{
				auto nextUsedUnit = unit2tex.lower_bound(smallestFreeUnit);
				while (smallestFreeUnit == nextUsedUnit->first)
				{
					smallestFreeUnit++;
					nextUsedUnit++;
				}
			}

			BindTexture(tr.unit, tex);

			return tr.unit;
		}
		// CASE 3: The texture is not in cache, and the cache is full.
		// Evict the least recently used texture
		else
		{
			// Find lru texture
			Texture* lru = NULL;
			TexRecord lrurec; lrurec.lastTouched = touch;
			for (auto it = tex2rec.begin(); it != tex2rec.end(); it++)
			{
				if (it->second.lastTouched < lrurec.lastTouched)
				{
					lrurec = it->second;
					lru = it->first;
				}
			}

			// Erase it
			tex2rec.erase(tex2rec.find(lru));

			// Insert the new texture
			TexRecord& newRec = tex2rec[tex];
			newRec.lastTouched = touch;
			newRec.unit = lrurec.unit;
			unit2tex[newRec.unit] = tex;

			// Bind
			BindTexture(newRec.unit, tex);

			// No need to update the smallest available unit;
			// they're all still occupied after this operation

			return newRec.unit;
		}

	}

	void TexUnitCache::RemoveFromCache(Texture* tex)
	{
		auto it = tex2rec.find(tex);
		if (it != tex2rec.end())
		{
			UINT unit = it->second.unit;
			tex2rec.erase(it);
			unit2tex.erase(unit2tex.find(unit));

			// Update the smallest available unit, if the
			// one we just freed up is smaller
			smallestFreeUnit = Min(unit, smallestFreeUnit);
		}
	}

////////////////////////////////////////////////////////////////////////////////////

	TexUnitCache* Texture::cache = new TexUnitCache;

	Texture::~Texture()
	{
		cache->RemoveFromCache(this);
	}

	UINT Texture::GetTextureUnit()
	{
		return cache->GetTextureUnit(this);
	}

	void Texture::Bind(const std::string& name)
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();

		UINT texUnit = GetTextureUnit();

		int loc = prog->GetUniformLocation(name);
		if (loc >= 0)
			glUniform1i(loc, texUnit);
	}

	void Texture::ApplySettings(const TextureSettings& settings)
	{
		UINT texUnit = GetTextureUnit();
		glActiveTexture(texUnit);

		// Wrapping
		switch(settings.wrapMode)
		{
		case TextureSettings::ClampWrap:
			glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;
		case TextureSettings::RepeatWrap:
			glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		default: break;
		}

		// Filtering
		switch(settings.filterMode)
		{
		case TextureSettings::NearestFilter:
			glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TextureSettings::LinearFilter:
			glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case TextureSettings::MipMapFilter:
			glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(texType);
			break;
		default: break;
		}
	}

	// Code adapted from the official FreeImage documentation
	FIBITMAP* Texture::LoadImage(const std::string& filename)
	{
		const char* fname = filename.c_str();

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		// check the file signature and deduce its format
		// (the second argument is currently not used by FreeImage)
		fif = FreeImage_GetFileType(fname, 0);
		if(fif == FIF_UNKNOWN)
		{
			// no signature ?
			// try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(fname);
		}
		// check that the plugin has reading capabilities ...
		if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
		{
			// ok, let's load the file
			FIBITMAP *dib = FreeImage_Load(fif, fname);
			// unless a bad file format, we are done !
			return dib;
		}

		FatalError(string("Texture::LoadImage - unable to load image '" + filename + "'"))
	}
}