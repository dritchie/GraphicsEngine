#pragma once

#ifndef GRAPHICSENGINE__GRAPHICS_CONTEXT_H
#define GRAPHICSENGINE__GRAPHICS_CONTEXT_H


#include "Common/Common.h"


namespace GraphicsEngine
{
	/**
	Interface to the OpenGL context and the window that owns it

	May support other options later as needed
	**/
	class GraphicsContext
	{
	public:
		virtual void Redraw() = 0;
		virtual void MakeCurrent() = 0;
	};


	/**
	Must be called before using any GraphicsEngine code (but after the context
	has been created, obviously)
	**/
	void Initialize(GraphicsContext* context);

	/**
	Call when done using GraphicsEngine code (i.e. end of program) to free up
	resources
	**/
	void Deinitialize();
}


#endif