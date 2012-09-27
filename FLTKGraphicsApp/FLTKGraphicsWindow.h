#pragma once

#ifndef FLTK_GRAPHICS_WINDOW_H
#define FLTK_GRAPHICS_WINDOW_H


#include "GraphicsApp.h"
#include "FL/Fl_Gl_Window.H"


/**
Subclass of Fl_Gl_Window that acts as graphics context for
a GraphicsEngine::GraphicsApp
**/
class FLTKGraphicsWindow : public Fl_Gl_Window, public GraphicsEngine::GraphicsContext
{
public:

	FLTKGraphicsWindow(GraphicsApp* gApp, int x, int y, int w, int h, const char *l);

	// Fl_Gl_Window overrides
	void draw();
	int handle(int eventType);

	// GraphicsContext overrides
	void Redraw();
	void MakeCurrent();

private:

	static int ConvertMouseButtonCode(int mouseButton);
	static int ConvertKeyCode(int key);

	int heldKey, heldButton;

	GraphicsApp* app;
};


#endif