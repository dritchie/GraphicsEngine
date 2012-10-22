#pragma once

#ifndef GRAPHICS_APP_H
#define GRAPHICS_APP_H


#include "Common/Common.h"
#include "Application/GraphicsAppParams.h"
#include "Application/Keys.h"
#include "Application/GraphicsContext.h"
#include "Viewing/Camera.h"


/**
Abstract base class for graphics applications
**/
class GraphicsApp
{
public:

	GraphicsApp();

	virtual void Init(const std::string& paramfile);
	virtual void InitGraphics(GraphicsEngine::GraphicsContext* ctx);
	virtual GraphicsEngine::GraphicsContext* InitAndShowUI(int argc, char** argv);

	virtual void Render() = 0;
	virtual void Resize(int w, int h);

	// Keyboard events
	virtual void KeyDown(int key, const GraphicsEngine::ModifierKeys& mods) = 0;
	virtual void KeyUp(int key, const GraphicsEngine::ModifierKeys& mods) = 0;

	// Mouse events
	virtual void MouseDown(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods) = 0;
	virtual void MouseUp(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods) = 0;
	virtual void MouseMove(int x, int y, const GraphicsEngine::ModifierKeys& mods) = 0;
	virtual void MouseDrag(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods) = 0;
	virtual void MouseWheel(int delta, const GraphicsEngine::ModifierKeys& mods) = 0;

	// The march of time...
	virtual void Timestep(float dt) = 0;

	// Call this static method in your main() to get everything running
	// Can only have one app at a time running; FLTK's event loop will take over once you call this
	static int Launch(GraphicsApp* app, const std::string& paramFilename, int argc, char** argv);

	GraphicsEngine::GraphicsAppParams params;

protected:

	GraphicsEngine::GraphicsContext* context;
};


#endif