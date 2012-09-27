#include "Assets/UniformSources.h"
#include "Assets/Texture/Texture2D.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Common/GL.h"

using namespace std;

namespace GraphicsEngine
{
	string UniformVec4Source::WrapInEvalFunction(const string& code)
	{
		return "vec4 " + name + "_Eval()\n{\n" + code + "\n}\n";
	}

	string UniformFloatSource::WrapInEvalFunction(const string& code)
	{
		return "float " + name + "_Eval()\n{\n" + code + "\n}\n";
	}

	void ConstantVec4Source::ShaderCode(ShaderAssetCode& code)
	{
		string var = "uniform vec4 " + name + ";";
		string eval = "return " + name + ";";
		code.fragCode.push_back(Block(name, var + "\n" + WrapInEvalFunction(eval)));
	}

	void ConstantVec4Source::Bind()
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();

		int varloc = prog->GetUniformLocation(name);
		if (varloc >= 0)
			glUniform4fv(varloc, 1, vec4.data());
	}

	void ConstantFloatSource::ShaderCode(ShaderAssetCode& code)
	{
		string var = "uniform float " + name + ";";
		string eval = "return " + name + ";";
		code.fragCode.push_back(Block(name, var + "\n" + WrapInEvalFunction(eval)));
	}

	void ConstantFloatSource::Bind()
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();

		int varloc = prog->GetUniformLocation(name);
		if (varloc >= 0)
			glUniform1f(varloc, fl);
	}

	void TextureVec4Source::ShaderCode(ShaderAssetCode& code)
	{
		code.vertCode.push_back(Define("PLUMB_UVS", ""));
		code.fragCode.push_back(Define("PLUMB_UVS", ""));
		string var = "uniform sampler2D " + name + ";";
		string eval = "return texture(" + name + ", UV);";
		code.fragCode.push_back(Block(name, var + "\n" + WrapInEvalFunction(eval)));
	}

	void TextureVec4Source::Bind()
	{
		tex->Bind(name);
	}

	void TextureFloatSource::ShaderCode(ShaderAssetCode& code)
	{
		code.vertCode.push_back(Define("PLUMB_UVS", ""));
		code.fragCode.push_back(Define("PLUMB_UVS", ""));
		string var = "uniform sampler2D " + name + ";";
		string eval = "return texture(" + name + ", UV).x;";
		code.fragCode.push_back(Block(name, var + "\n" + WrapInEvalFunction(eval)));
	}

	void TextureFloatSource::Bind()
	{
		tex->Bind(name);
	}
}