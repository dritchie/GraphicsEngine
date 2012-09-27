#include "FL/Fl.H"
#include "GraphicsApp.h"
#include "FLTKGraphicsWindow.h"
#include "Utility/Timer.h"


GraphicsApp app;

void idle(void* requiredArgument = NULL)
{
	static Timer timer;
	timer.stop();
	float dt = (float)(timer.getElapsedTimeInMicroSec() / 1000000.0f);
	app.Timestep(dt);
	timer.start();
}

int main(int argc, char** argv)
{
	// Initialize the app
	app.Init("Parameters.txt");

	// Initialize the UI (Which creates the OpenGL context)
	FLTKGraphicsWindow wind(&app, 200, 200, 800, 600, "FLTKGraphicsApp");
	wind.end();
	wind.resizable(wind);
	wind.show(argc, argv);

	// Initialize GraphicsEngine
	GraphicsEngine::Initialize(&wind);

	// Initialize application graphics components
	app.InitGraphics(&wind);

	// Set the idle process
	Fl::add_idle(idle);

	// Run it!
	int retval = Fl::run();

	// Clean up and return
	GraphicsEngine::Deinitialize();
	return retval;
}