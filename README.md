GraphicsEngine
==============

Framework for getting graphics applications up and running. A level of abstraction (or two, or three) up from OpenGL.

## Dependencies

- glew (http://glew.sourceforge.net) - Tested against version 1.7.0
- Eigen (http://eigen.tuxfamily.org) - Tested against version 3.1.1
- assimp (http://assimp.sourceforge.net) - Tested against version 2.0
- FreeImage (http://freeimage.sourceforge.net) - Tested against version 3.15.3
- (Optional) FLTK (http://www.fltk.org) - Tested against version 1.3.0

## Building

Currently, the only build infrastructure available is Visual Studio 2010. However, the code is (in principle) cross-platform.

### VS 2010

GraphicsEngine.sln should be self-explanatory.

Ensure that Visual Studio can find the libs and headers for the dependencies. Adding the necessary paths to the 'Microsoft.Cpp.Win32.User' property sheet is one way to do this. If you built FreeImage and/or GLEW as dynamic libraries, make sure that those .dlls are in your system Path (for command-line execution) and/or your Visual Studio bin directory (for running from within Visual Studio).

### FastSTL

assimp, by default, compiles with STL iterator debugging disabled. This helps speed up a lot of file loading operations done using STL classes. It also means that any code compiled with iterator debugging enabled will be incompatible with assimp. This is where the 'FastSTL' property sheet comes in: any projects that link against GraphicsEngine (which links against assimp) should include FastSTL.props. Otherwise, things will probably explode. I'm sure there's some corresponding action that must be taken when building on Unix-y systems; probably best to refer to the assimp install documentation.

Note that it is also possible to compile assimp with these settings removed (at a cost to performance, obviously) if you want to avoid all of this.

## Usage

**GraphicsEngine.lib** is a library containing various useful real-time graphics utilities (mesh loading/generation, shader loading/generation, texture loading, etc.)

**FLTKGraphicsApp** is a library that uses GraphicsEngine, launching an OpenGL window using FLTK. Applications can extend the functionality of the basic app to do whatever they desire. Note that an installation of FLTK is only required if you want to use this; if you have a separate windowing/input management system and you just want to use GraphicsEngine, FLTK is not necessary.

See the 'Examples' directory for a couple of (admittedly rather shoddy) examples of how this stuff works. It could use a lot better documentation (especially the uber shader stuff), but I have no plans to write this any time soon.