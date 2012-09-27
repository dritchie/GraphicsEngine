#include "Assets/Light/MultiLightSystem.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Assets/Light/Light.h"
#include "Common/GL.h"
#include "Math/Math.h"

using namespace std;

namespace GraphicsEngine
{

	MultiLightSystem::MultiLightSystem()
		: lastMaxNumLights(0)
	{
	}

	void MultiLightSystem::Push(Light* light, const Transform& xform)
	{
		theStack.push_front(make_pair(light, xform));
	}

	void MultiLightSystem::Pop()
	{
		theStack.pop_front();
	}

	void MultiLightSystem::Clear()
	{
		theStack.clear();
	}

	void MultiLightSystem::ShaderCode(ShaderAssetCode& code)
	{
		lastMaxNumLights = RoundUpPow2((int)(theStack.size()));
		char numLights[16];
		SafePrintf(numLights, 16, "%d", lastMaxNumLights);

		vector<string> lines;
		lines.push_back(Define("MAX_NUM_LIGHTS", numLights));
		lines.push_back("#include \"Multi.LightSystem\"");
		code.fragCode.push_back(Block("LIGHT_SYSTEM", lines));
	}

	void MultiLightSystem::Bind()
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();
		int numLoc = prog->GetUniformLocation("NumLights");
		if (numLoc >= 0)
			glUniform1i(numLoc, (int)(theStack.size()));

		for (UINT i = 0; i < theStack.size(); i++)
		{
			char lightName[16];
			SafePrintf(lightName, 16, "lights[%d]", i);
			theStack[i].first->Bind(lightName, theStack[i].second);
		}
	}

	bool MultiLightSystem::CodeChanged()
	{
		int maxNumLights = RoundUpPow2((int)(theStack.size()));
		return maxNumLights != lastMaxNumLights;
	}
}