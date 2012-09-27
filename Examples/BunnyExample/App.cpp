#include "App.h"
#include "Common/GL.h"
#include "Viewing/FPSCamera.h"
//#include "Viewing/MayaCamera.h"
#include "Math/TransformStack.h"

#include "Math/Transform.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Assets/Mesh/CommonMesh.h"

using namespace std;
using namespace GraphicsEngine;
using Eigen::Vector3f;
using Eigen::Vector4f;


CommonMesh* mesh;
Shader *vs, *fs;
ShaderProgram* prog;


App::App()
{
}

App::~App()
{
	SAFEDELETE(camera);
	SAFEDELETE(mesh);
	SAFEDELETE(prog);
	SAFEDELETE(vs);
	SAFEDELETE(fs);
}

void App::Init(const string& paramfile)
{
	GraphicsApp::Init(paramfile);
	InitCamera();
}

void App::InitGraphics(GraphicsContext* ctx)
{
	GraphicsApp::InitGraphics(ctx);

	/** TEST STUFF **/

	// Shader
	vs = new Shader(Shader::VertexShader); vs->FromFile("SimplePhong.vert");
	fs = new Shader(Shader::FragmentShader); fs->FromFile("SimplePhong.frag");
	vector<Shader*> shaders; shaders.push_back(vs); shaders.push_back(fs);
	prog = new ShaderProgram(shaders);
	prog->Bind();

	// Load mesh
	mesh = new CommonMesh; mesh->Load("bunny.obj", true);
}

void App::InitCamera()
{
	FPSCamera* fpsCam = new FPSCamera(Vector3f::Zero(), -5.0f*Vector3f::UnitZ(), Vector3f::UnitY());
	fpsCam->InvertY() = true;
	camera = fpsCam;

	//MayaCamera* mayaCam = new MayaCamera(Vector3f::Zero(), -5.0f*Vector3f::UnitZ(), Vector3f::UnitY());
	//camera = mayaCam;
}

void App::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	TransformStack::Modelview().Load(camera->GetLookAtTransform());

	Transform t = Transform::Translation(0.0f, -2.0f, -5.0f) * Transform::Scale(20.0f);
	TransformStack::Modelview().Multiply(t);

	TransformStack::Modelview().Bind();
	TransformStack::Projection().Bind();
	mesh->Render();
}

void App::KeyDown(int key, const ModifierKeys& mods)
{
	if (camera->KeyDown(key, mods))
		context->Redraw();
}

void App::KeyUp(int key, const ModifierKeys& mods)
{
	if (camera->KeyUp(key, mods))
		context->Redraw();
}

void App::MouseDown(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods)
{
	if (camera->MouseDown(button, x, y, mods))
		context->Redraw();
}

void App::MouseUp(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods)
{
	if (camera->MouseUp(button, x, y, mods))
		context->Redraw();
}

void App::MouseMove(int x, int y, const ModifierKeys& mods)
{
	if (camera->MouseMove(x, y, mods))
		context->Redraw();
}

void App::MouseDrag(int button, int x, int y, const ModifierKeys& mods)
{
	if (camera->MouseDrag(button, x, y, mods))
		context->Redraw();
}

void App::MouseWheel(int delta, const ModifierKeys& mods)
{	
	if (camera->MouseWheel(delta, mods))
		context->Redraw();
}

void App::Timestep(float dt)
{
	if (camera->Timestep(dt))
		context->Redraw();
}