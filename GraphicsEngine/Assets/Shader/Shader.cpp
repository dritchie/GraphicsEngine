#include "Assets/Shader/Shader.h"
#include "Utility/FileSystem.h"
#include "Utility/MacroPreprocessor.h"
#include "Common/GL.h"

using namespace std;

namespace GraphicsEngine
{
	string Shader::shaderPath = "";

	Shader::Shader(ShaderStage sstage)
		: stage(sstage)
	{
	}

	Shader::~Shader()
	{
		glDeleteShader(shaderID);
	}

	void Shader::FromFile(const std::string& filename)
	{
		MacroPreprocessor parser(shaderPath);
		parser.InitFromFile(filename);
		parser.CompleteParse();
		FromParsedSource(parser.GetParseResults());
	}

	void Shader::FromSource(const std::string& sourceCode)
	{
		MacroPreprocessor parser(shaderPath);
		parser.InitFromSource(sourceCode);
		parser.CompleteParse();
		FromParsedSource(parser.GetParseResults());
	}

	void Shader::FromParsedSource(const std::string& src)
	{
		// Spit out the fully parsed source into a temporary file
		// (Helps debug any parsing errors)
		SaveTextToFile(src, "shader_tmp");

		int stageflag;
		if (stage == VertexShader)
			stageflag = GL_VERTEX_SHADER;
		if (stage == TessellationControlShader)
			stageflag = GL_TESS_CONTROL_SHADER;
		if (stage == TessellationEvalShader)
			stageflag = GL_TESS_EVALUATION_SHADER;
		if (stage == GeometryShader)
			stageflag = GL_GEOMETRY_SHADER;
		if (stage == FragmentShader)
			stageflag = GL_FRAGMENT_SHADER;

		shaderID = glCreateShader(stageflag);

		const char* srcText = src.c_str();

	    glShaderSource(shaderID, 1, &srcText, 0);

		glCompileShader(shaderID);

		// Check for compile errors
		ConfirmCompile();
	}

	void Shader::ConfirmCompile()
	{
		int compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			int logLength;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(shaderID, logLength, NULL, infoLog);
			string errorMsg = "GraphicsEngine::ShaderProgram - GLSL Compiler error(s):\n\n" + string(infoLog);
			delete infoLog;
			FatalError(errorMsg)
		}
	}

	string Shader::LoadSource(const string& filename)
	{
		vector<string> paths = SplitString(shaderPath, ";");

		// Try to find the file in the path
		string resolvedname;
		bool foundit = false;
		for (UINT i = 0; i < paths.size(); i++)
		{
			resolvedname = paths[i] + filename;
			if (FileExists(resolvedname))
			{
				foundit = true;
				break;
			}
		}
		if (!foundit)
			FatalError(string("Shader::LoadSource - Could not find file '" + filename + "'"))

		return TextFromFile(resolvedname);
	}
}