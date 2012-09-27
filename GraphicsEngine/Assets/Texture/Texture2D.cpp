#include "Assets/Texture/Texture2D.h"
#include "Common/GL.h"
#include "FreeImage.h"

using namespace std;

namespace GraphicsEngine
{
	Texture2D::Texture2D(const string& filename, const TextureSettings& settings)
	{
		texType = GL_TEXTURE_2D;
		glGenTextures(1, &texName);

		FIBITMAP* bitmap = LoadImage(filename);
		UINT w = FreeImage_GetWidth(bitmap);
		UINT h = FreeImage_GetHeight(bitmap);
		int type, format, internalFormat;
		void* data = PrepareForGL(bitmap, internalFormat, format, type);
		if (data == NULL)
		{
			FatalError(string("Texture2D - Image file '" + filename + "' has unsupported type (Texture2D only supports 8-bit grayscale OR 8-bit, 16-bit, and floating-point RGB[A])"))
		}
		FreeImage_Unload(bitmap);

		UINT texUnit = GetTextureUnit();
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glTexImage2D(texType, 0, internalFormat, w, h, 0, format, type, data);
		delete[] data;

		ApplySettings(settings);
	}

	void* Texture2D::PrepareForGL(FIBITMAP* img, int& internalFormat, int& format, int& type)
	{
		UINT w, h;
		w = FreeImage_GetWidth(img);
		h = FreeImage_GetHeight(img);

		FREE_IMAGE_TYPE fit = FreeImage_GetImageType(img);
		UINT bpp = FreeImage_GetBPP(img);

		// CASE: 8-bit grayscale
		if (fit == FIT_BITMAP && bpp == 8)
		{
			unsigned char* data = new unsigned char[w*h*3];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				BYTE* pixels = FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x];
					data[i+1] = pixels[x];
					data[i+2] = pixels[x];
					i += 3;
				}
			}
			internalFormat = GL_RGB8;
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			return data;
		}
		// CASE: 8-bit RGB
		else
		if (fit == FIT_BITMAP && bpp == 24)
		{
			unsigned char* data = new unsigned char[w*h*3];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				RGBTRIPLE* pixels = (RGBTRIPLE*)FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x].rgbtRed;
					data[i+1] = pixels[x].rgbtGreen;
					data[i+2] = pixels[x].rgbtBlue;
					i += 3;
				}
			}
			internalFormat = GL_RGB8;
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			return data;
		}
		// CASE: 8-bit RGBA
		else
		if (fit == FIT_BITMAP && bpp == 32)
		{
			unsigned char* data = new unsigned char[w*h*4];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				RGBQUAD* pixels = (RGBQUAD*)FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x].rgbRed;
					data[i+1] = pixels[x].rgbGreen;
					data[i+2] = pixels[x].rgbBlue;
					data[i+3] = pixels[x].rgbReserved;
					i += 4;
				}
			}
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			return data;
		}
		// CASE: 16-bit RGB
		else
		if (fit == FIT_RGB16)
		{
			unsigned short* data = new unsigned short[w*h*3];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				FIRGB16* pixels = (FIRGB16*)FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x].red;
					data[i+1] = pixels[x].green;
					data[i+2] = pixels[x].blue;
					i += 3;
				}
			}
			internalFormat = GL_RGB16;
			format = GL_RGB;
			type = GL_UNSIGNED_SHORT;
			return data;
		}
		// CASE: 16-bit RGBA
		else
		if (fit == FIT_RGBA16)
		{
			unsigned short* data = new unsigned short[w*h*4];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				FIRGBA16* pixels = (FIRGBA16*)FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x].red;
					data[i+1] = pixels[x].green;
					data[i+2] = pixels[x].blue;
					data[i+3] = pixels[x].alpha;
					i += 4;
				}
			}
			internalFormat = GL_RGBA16;
			format = GL_RGBA;
			type = GL_UNSIGNED_SHORT;
			return data;
		}
		// CASE: Floating-point RGB
		else
		if (fit == FIT_RGBF)
		{
			float* data = new float[w*h*3];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				FIRGBF* pixels = (FIRGBF*)FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x].red;
					data[i+1] = pixels[x].green;
					data[i+2] = pixels[x].blue;
					i += 3;
				}
			}
			internalFormat = GL_RGB32F;
			format = GL_RGB;
			type = GL_FLOAT;
			return data;
		}
		// CASE: Floating-point RGBA
		else
		if (fit == FIT_RGBAF)
		{
			float* data = new float[w*h*4];
			UINT i = 0;
			for (UINT y = 0; y < h; y++)
			{
				FIRGBAF* pixels = (FIRGBAF*)FreeImage_GetScanLine(img, y);
				for (UINT x = 0; x < w; x++)
				{
					data[i] = pixels[x].red;
					data[i+1] = pixels[x].green;
					data[i+2] = pixels[x].blue;
					data[i+3] = pixels[x].alpha;
					i += 4;
				}
			}
			internalFormat = GL_RGBA32F;
			format = GL_RGBA;
			type = GL_FLOAT;
			return data;
		}
		// Any other image type is unrecognized; return NULL
		else
		{
			return NULL;
		}
	}
}
