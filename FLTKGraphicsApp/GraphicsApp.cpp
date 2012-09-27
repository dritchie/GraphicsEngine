#include "Common/GL.h"
#include "GraphicsApp.h"
#include "Math/TransformStack.h"
#include "Assets/Shader/Shader.h"
#include "Utility/Timer.h"
#include "FLTKGraphicsWindow.h"
#include "FL/Fl.H"


using namespace std;
using namespace GraphicsEngine;


// I *HATE* having this global variable, but since FLTK::add_idle
// expects a function pointer, I have no other choice. Lambdas are
// not type-equivalent to function pointers, so I'm stuck.
GraphicsApp* gApp = NULL;

void idle(void* requiredArgument = NULL)
{
	static Timer timer;
	timer.stop();
	float dt = (float)(timer.getElapsedTimeInMicroSec() / 1000000.0f);
	gApp->Timestep(dt);
	timer.start();
}

int GraphicsApp::Launch(GraphicsApp* app, const std::string& paramFilename, int argc, char** argv)
{
	// Initialize the app
	app->Init("Parameters.txt");

	// Initialize the UI (Which creates the OpenGL context)
	FLTKGraphicsWindow wind(app, 200, 200, app->params.IntParam("windowWidth"),
		app->params.IntParam("windowHeight"), app->params.StringParam("appName").c_str());
	wind.end();
	wind.resizable(wind);
	wind.show(argc, argv);

	// Initialize GraphicsEngine
	GraphicsEngine::Initialize(&wind);

	// Initialize application graphics components
	app->InitGraphics(&wind);

	// Set the idle process
	Fl::add_idle(idle);

	// Run it!
	int retval = Fl::run();

	// Clean up and return
	GraphicsEngine::Deinitialize();
	return retval;
}


GraphicsApp::GraphicsApp()
	: context(NULL)
{
}


void GraphicsApp::Init(const string& paramfile)
{
	params.Init(paramfile);
	Shader::ShaderPath(params.StringParam("shaderPath"));
}

void GraphicsApp::InitGraphics(GraphicsContext* ctx)
{
	context = ctx;
	glEnable(GL_DEPTH_TEST);
}

void GraphicsApp::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	TransformStack::Projection().Load(Transform::Perspective((float)params.FloatParam("fovy"),
															 ((float)w/h),
															 (float)params.FloatParam("znear"),
															 (float)params.FloatParam("zfar")));
}