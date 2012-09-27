#pragma once

#ifndef GRAPHICS_APP_H
#define GRAPHICS_APP_H


#include "Common/Common.h"
#include "Application/GraphicsAppParams.h"
#include "Application/Keys.h"
#include "Application/GraphicsContext.h"
#include "Viewing/Camera.h"


// Remove this
namespace GraphicsEngine
{
	class CommonMesh;
}


/**
Main application logic goes here
**/
class GraphicsApp
{
public:

	GraphicsApp();
	~GraphicsApp();

	void Init(const std::string& paramfile);
	void InitGraphics(GraphicsEngine::GraphicsContext* ctx);

	void Render();
	void Resize(int w, int h);

	// Keyboard events
	void KeyDown(int key, const GraphicsEngine::ModifierKeys& mods);
	void KeyUp(int key, const GraphicsEngine::ModifierKeys& mods);

	// Mouse events
	void MouseDown(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods);
	void MouseUp(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods);
	void MouseMove(int x, int y, const GraphicsEngine::ModifierKeys& mods);
	void MouseDrag(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods);
	void MouseWheel(int delta, const GraphicsEngine::ModifierKeys& mods);

	void Timestep(float dt);

	GraphicsEngine::GraphicsAppParams params;

private:

	void InitCamera();

	GraphicsEngine::GraphicsContext* context;
	GraphicsEngine::InteractiveCamera* camera;
};


#endif