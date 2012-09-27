#include "Assets/Shader/UberShader.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Shader/ShaderProgram.h"

using namespace std;

namespace GraphicsEngine
{
	UberShader::UberShader(const string& vertFile,
						   const string& fragFile,
						   const string& geoFile,
						   const string& tessControlFile,
						   const string& tessEvalFile)
	: vertSrc(""), fragSrc(""), geoSrc(""), tcSrc(""), teSrc("")
	{
		vertSrc = Shader::LoadSource(vertFile);
		fragSrc = Shader::LoadSource(fragFile);

		if (!geoFile.empty())
		{
			geoSrc = Shader::LoadSource(geoFile);
		}

		if (!tessControlFile.empty())
		{
			tcSrc = Shader::LoadSource(tessControlFile);
		}

		if (!tessEvalFile.empty())
		{
			teSrc = Shader::LoadSource(tessEvalFile);
		}
	}

	UberShader::~UberShader()
	{
		ClearCache();
	}

	__forceinline string ConcatVector(const vector<string>& strset)
	{
		string result = "";
		for (auto it = strset.begin(); it != strset.end(); it++)
			result += *it + "\n";
		return result;
	}

	string ConcatCode(const ShaderAssetCode& code)
	{
		return ConcatVector(code.vertCode) +
			   ConcatVector(code.geoCode) +
			   ConcatVector(code.tessControlCode) +
			   ConcatVector(code.tessEvalCode) +
			   ConcatVector(code.fragCode);
	}

	ShaderProgram* UberShader::GetProgram(const ShaderAssetCode& code)
	{
		string codestr = ConcatCode(code);
		auto it = code2program.find(codestr);
		if (it != code2program.end())
			return it->second;
		else
		{
			// Compile a new program
			return Compile(code);
		}
	}

	ShaderProgram* UberShader::Compile(const ShaderAssetCode& code)
	{
		auto ShaderConditionalCompile = [this]
		(const string& code, const string& src, Shader::ShaderStage stage) -> Shader*
		{
			string fullsrc = code + src;
			Shader* shader = new Shader(stage);
			shader->FromSource(fullsrc);
			code2shader[code] = shader;
			return shader;
		};

		auto GetShader = [this, &ShaderConditionalCompile]
		(const vector<string>& code, const string& src, Shader::ShaderStage stage) -> Shader*
		{
			string codestr = ConcatVector(code);
			auto it = code2shader.find(codestr);
			if (it != code2shader.end())
				return it->second;
			else
			{
				// Compile a new shader
				return ShaderConditionalCompile(codestr, src, stage);
			}
		};

		vector<Shader*> shaders;
		shaders.push_back(GetShader(code.vertCode, vertSrc, Shader::VertexShader));
		shaders.push_back(GetShader(code.fragCode, fragSrc, Shader::FragmentShader));
		if (!geoSrc.empty())
			shaders.push_back(GetShader(code.geoCode, geoSrc, Shader::GeometryShader));
		if (!tcSrc.empty())
			shaders.push_back(GetShader(code.tessControlCode, tcSrc, Shader::TessellationControlShader));
		if (!teSrc.empty())
			shaders.push_back(GetShader(code.tessEvalCode, teSrc, Shader::TessellationEvalShader));

		ShaderProgram* prog = new ShaderProgram(shaders);
		string codestr = ConcatCode(code);
		code2program[codestr] = prog;
		for (UINT i = 0; i < shaders.size(); i++)
			shader2programs[shaders[i]].insert(prog);

		return prog;
	}

	void UberShader::DeleteProgram(ShaderProgram* prog)
	{
		// Delete program from the defs2program cache
		for (auto it = code2program.begin(); it != code2program.end(); it++)
		{
			if (it->second == prog)
			{
				code2program.erase(it);
				break;
			}
		}

		// For each shader that the program depends on, remove that program
		// from shader2programs. If the shader now has zero associated programs,
		// also delete that shader.
		for (UINT i = 0; i < prog->shaders.size(); i++)
		{
			Shader* shader = prog->shaders[i];
			auto it = shader2programs.find(shader);
			it->second.erase(it->second.find(prog));
			CheckedDeleteShader(shader);
		}

		// Actually delete the program
		delete prog;
	}

	void UberShader::CheckedDeleteShader(Shader* shader)
	{
		// Delete the shader and remove from cache, provided
		// it has no associated programs.
		auto it = shader2programs.find(shader);
		if (it->second.empty())
		{
			shader2programs.erase(it);
			auto it2 = code2shader.begin();
			for (auto it2 = code2shader.begin(); it2 != code2shader.end(); it2++)
			{
				if (it2->second == shader)
				{
					code2shader.erase(it2);
					break;
				}
			}
			delete shader;
		}
	}

	void UberShader::ClearCache()
	{
		// Form lists of all programs and all shaders
		vector<ShaderProgram*> progs;
		vector<Shader*> shaders;
		for (auto it = code2program.begin(); it != code2program.end(); it++)
			progs.push_back(it->second);
		for (auto it = code2shader.begin(); it != code2shader.end(); it++)
			shaders.push_back(it->second);

		// Delete everything (programs first, to be safe)
		for (UINT i = 0; i < progs.size(); i++)
			DeleteProgram(progs[i]);
		for (UINT i = 0; i < shaders.size(); i++)
			CheckedDeleteShader(shaders[i]);
	}
}