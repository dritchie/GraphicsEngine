#pragma once

#ifndef GRAPHICS_ENGINE__TEXTURE_H
#define GRAPHICS_ENGINE__TEXTURE_H

#include "Common/Common.h"

struct FIBITMAP;

namespace GraphicsEngine
{
	/**
	Common settings that apply to different types of textures
	**/
	struct TextureSettings
	{
		enum FilterMode
		{
			NearestFilter = 0,
			LinearFilter,
			MipMapFilter
		};

		enum WrapMode
		{
			RepeatWrap = 0,
			ClampWrap
		};

		FilterMode filterMode;
		WrapMode wrapMode;

		TextureSettings(FilterMode f = MipMapFilter, WrapMode w = RepeatWrap)
			: filterMode(f), wrapMode(w) {}
	};


	/**
	Abstract base class for all textures. Texture objects are higher-level
	abstractions for OpenGL texture objects/samplers of various types.
	**/
	class Texture
	{
	public:

		virtual ~Texture();

		// Bind this texture to a shader uniform with the given name
		virtual void Bind(const std::string& name);

		friend class TexUnitCache;

	protected:

		// Retrieves the texture unit this texture is currently bound to.
		// If this texture has never been bound, or if it has been evicted from
		// the texture unit cache, this function first binds the texture to some
		// texture unit.
		// NOTE: Due to cache eviction, the return value of this function can change
		// at any time. Thus, do not store the return value. Instead, call this function
		// to retrieve the value fresh before invoking any other operations that require
		// the texture unit.
		UINT GetTextureUnit();

		// Applies the provided settings to this texture/the associated texture unit
		// NOTE: Texture data must be loaded before calling this function (i.e. this should
		// be the last thing called in texture initialization)
		void ApplySettings(const TextureSettings& settings);

		// Use FreeImage to load image data from a file
		static FIBITMAP* LoadImage(const std::string& filename);

		UINT texType;	// i.e. GL_TEXTURE_2D
		UINT texName;	// i.e. value returned by glGenTexture

	private:
		
		static TexUnitCache* cache;
	};
}

#endif