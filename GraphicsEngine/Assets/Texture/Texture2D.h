#pragma once

#ifndef GRAPHICS_ENGINE__TEXTURE_2D_H
#define GRAPHICS_ENGINE__TEXTURE_2D_H

#include "Assets/Texture/Texture.h"

namespace GraphicsEngine
{
	/**
	A 2-dimensional image texture. The most common type of
	graphics texture
	**/
	class Texture2D : public Texture
	{
	public:

		Texture2D(const std::string& filename, const TextureSettings& settings = TextureSettings());

	private:

		// Convert a FreeImage img into a format ready for OpenGL texturing. Returns NULL if
		// the conversion could not be performed.
		// Also fill in the correct internalFormat, format, and type for OpenGL texturing.
		// NOTE: The caller is reponsible for deallocating the returned pointer (via delete[])
		static void* PrepareForGL(FIBITMAP* img, int& internalFormat, int& format, int& type);
	};
}


#endif