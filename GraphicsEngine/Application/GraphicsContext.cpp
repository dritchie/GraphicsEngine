#include "Application/GraphicsContext.h"
#include "Common/GL.h"
#include "FreeImage.h"

using namespace std;


namespace GraphicsEngine
{
	void Initialize(GraphicsContext* context)
	{
		if (context == NULL)
			FatalError(string("GraphicsEngine::Initialize - Provided OpenGL context is NULL!"))

		// Make context the current context
		try
		{
			context->MakeCurrent();
		}
		catch(...)
		{
			FatalError(string("GraphicsEngine::Initialize - Provided OpenGL context could not be made the current context. Are you sure it was properly initialized?"))
		}

		// Initialize GLEW
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			const GLubyte* errstr = glewGetErrorString(err);
			string errmsg = "GraphicsEngine::Initialize - GLEW failed to initialize:\n\n" + string((const char*)errstr);
			FatalError(errmsg)
		}

		// Initialize FreeImage
#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
#endif
	}

	void Deinitialize()
	{
#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
#endif
	}
}