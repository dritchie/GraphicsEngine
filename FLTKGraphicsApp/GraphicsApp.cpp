#include "Common/GL.h"
#include "GraphicsApp.h"
#include "Viewing/FPSCamera.h"
//#include "Viewing/MayaCamera.h"
#include "Math/TransformStack.h"

// TEST Remove these
#include "Math/Transform.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Assets/Shader/UberShader.h"
#include "Assets/Mesh/CommonMesh.h"
#include "Assets/UniformSources.h"
#include "Assets/Texture/Texture2D.h"
#include "Assets/Material/PhongBSDF.h"
#include "Assets/Material/ScatteringMaterial.h"
#include "Assets/Material/SmoothDiffGeo.h"
#include "Assets/Light/Light.h"
#include "Assets/Light/MultiLightSystem.h"
#include "Utility/MacroPreprocessor.h"

using namespace std;
using namespace GraphicsEngine;
using Eigen::Vector3f;
using Eigen::Vector4f;

// TEST Remove these
CommonMesh* mesh;
UberShader* us;
ShaderProgram* prog;
Texture2D* tex;
Material* mat;
Light light;
MultiLightSystem lightSys;

GraphicsApp::GraphicsApp()
	: context(NULL)
{
}

GraphicsApp::~GraphicsApp()
{
	SAFEDELETE(camera);
}

void GraphicsApp::Init(const string& paramfile)
{
	params.Init(paramfile);
	Shader::ShaderPath(params.StringParam("shaderPath"));
	InitCamera();
}

void GraphicsApp::InitGraphics(GraphicsContext* ctx)
{
	context = ctx;
	glEnable(GL_DEPTH_TEST);

	/** TEST STUFF **/

	//// Uber shader
	us = new UberShader("Uber.vert", "Uber.frag");

	//// Texture
	tex = new Texture2D("checkerboard_256.jpg");

	//// Material
	PhongBSDF* phong = new PhongBSDF(new ConstantVec4Source(Vector4f(0.0f, 0.0f, 0.1f, 1.0f)),
									 //new ConstantVec4Source(Vector4f(0.0f, 0.0f, 0.9f, 1.0f)),
									 new TextureVec4Source(tex),
									 new ConstantVec4Source(Vector4f(1.0f, 1.0f, 1.0f, 1.0f)),
									 new ConstantFloatSource(30.0f));
	SmoothDiffGeo* dg = new SmoothDiffGeo;
	mat = new ScatteringMaterial(phong, dg);

	//// Lights
	light.lightType = Light::PointLight;
	//light.lightType = Light::DirectionalLight;
	light.pos = Vector3f(4.0f, 4.0f, 4.0f);
	//light.dir = Vector3f(-1.0f, -1.0f, -1.0f);
	//light.spotCutoff = 60.0f;
	//light.spotExp = 45.0f;
	light.col = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//// Shader program
	lightSys.Push(&light, Transform::Identity());
	ShaderAssetCode code;
	lightSys.ShaderCode(code);
	mat->ShaderCode(code);
	prog = us->GetProgram(code);

	//// Mesh
	mesh = CommonMesh::CreatePlane(4.0f, 4.0f, 1, 1);
	//mesh = CommonMesh::CreateBox(4.0f, 4.0f, 4.0f, 4, 4);
	//mesh = CommonMesh::CreateCylinder(2.0f, 4.0f, 16, 4, true);
	//mesh = CommonMesh::CreateDisc(0.5f, 2.0f, 32);
	//mesh = CommonMesh::CreateSphere(2.0f, 32, 32);
	//mesh = new CommonMesh; mesh->Load("bunny.obj", true);
}

void GraphicsApp::InitCamera()
{
	FPSCamera* fpsCam = new FPSCamera(Vector3f::Zero(), -5.0f*Vector3f::UnitZ(), Vector3f::UnitY());
	fpsCam->InvertY() = true;
	camera = fpsCam;

	//MayaCamera* mayaCam = new MayaCamera(Vector3f::Zero(), -5.0f*Vector3f::UnitZ(), Vector3f::UnitY());
	//camera = mayaCam;
}

void GraphicsApp::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Clear stuff
	TransformStack::Modelview().Load(camera->GetLookAtTransform());
	lightSys.Clear();

	//// Update transforms
	Transform t = Transform::Translation(0.0f, 0.0f, -5.0f);
	//Transform t = Transform::Translation(0.0f, -2.0f, -5.0f) * Transform::Scale(20.0f);
	TransformStack::Modelview().Multiply(t);
	lightSys.Push(&light, TransformStack::Modelview().Top());

	//// Bind all assets
	prog->Bind();
	mat->Bind();
	lightSys.Bind();
	TransformStack::Modelview().Bind();
	TransformStack::Projection().Bind();

	//// Render the mesh
	mesh->Render();
}

void GraphicsApp::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	TransformStack::Projection().Load(Transform::Perspective((float)params.FloatParam("fovy"),
															 ((float)w/h),
															 (float)params.FloatParam("znear"),
															 (float)params.FloatParam("zfar")));
}

void GraphicsApp::KeyDown(int key, const ModifierKeys& mods)
{
	//cout << "GraphicsApp::KeyDown (key = " << key << ")" << endl;

	if (camera->KeyDown(key, mods))
		context->Redraw();
}

void GraphicsApp::KeyUp(int key, const ModifierKeys& mods)
{
	//cout << "GraphicsApp::KeyUp (key = " << key << ")" << endl;

	if (camera->KeyUp(key, mods))
		context->Redraw();
}

void GraphicsApp::MouseDown(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods)
{
	//cout << "Mouse down: "  << button << endl;

	if (camera->MouseDown(button, x, y, mods))
		context->Redraw();
}

void GraphicsApp::MouseUp(int button, int x, int y, const GraphicsEngine::ModifierKeys& mods)
{
	//cout << "Mouse up: "  << button << endl;

	if (camera->MouseUp(button, x, y, mods))
		context->Redraw();
}

void GraphicsApp::MouseMove(int x, int y, const ModifierKeys& mods)
{
	//cout << "Mouse move: "  << x << " " << y << " " << endl;

	if (camera->MouseMove(x, y, mods))
		context->Redraw();
}

void GraphicsApp::MouseDrag(int button, int x, int y, const ModifierKeys& mods)
{
	//cout << "Mouse drag: "  << button << " " << x << " " << y << " " << endl;

	if (camera->MouseDrag(button, x, y, mods))
		context->Redraw();
}

void GraphicsApp::MouseWheel(int delta, const ModifierKeys& mods)
{
	//cout << "Mouse wheel: "  << endl;
	
	if (camera->MouseWheel(delta, mods))
		context->Redraw();
}

void GraphicsApp::Timestep(float dt)
{
	//cout << "Timestep: " << dt << endl;

	if (camera->Timestep(dt))
		context->Redraw();
}