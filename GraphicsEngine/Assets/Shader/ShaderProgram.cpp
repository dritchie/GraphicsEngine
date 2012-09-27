#include "Assets/Shader/ShaderProgram.h"
#include "Assets/Shader/Shader.h"
#include "Common/GL.h"
#include "Utility/FileSystem.h"
#include <set>
#include <fstream>
#include <sstream>


using namespace std;


namespace GraphicsEngine
{
	ShaderProgram* ShaderProgram::currentProgram = NULL;

	ShaderProgram::ShaderProgram(const std::vector<Shader*>& componentShaders)
		: shaders(componentShaders)
	{
		progID = glCreateProgram();
		for (UINT i = 0; i < shaders.size(); i++)
		{
			glAttachShader(progID, shaders[i]->shaderID);
		}
		glLinkProgram(progID);

		// Check for link errors
		ConfirmLink();
	}

	ShaderProgram::~ShaderProgram()
	{
		for (UINT i = 0; i < shaders.size(); i++)
		{
			glDetachShader(progID, shaders[i]->shaderID);
		}

		glDeleteProgram(progID);
	}

	void ShaderProgram::ConfirmLink()
	{
		int linkStatus;
		glGetProgramiv(progID, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			int logLength;
			glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetProgramInfoLog(progID, logLength, NULL, infoLog);
			string errorMsg = "GraphicsEngine::ShaderProgram - GLSL Linker error(s):\n\n" + string(infoLog);
			delete[] infoLog;
			FatalError(errorMsg)
		}
	}

	int ShaderProgram::GetAttributeLocation(const std::string& attrib)
	{
		return glGetAttribLocation(progID, attrib.c_str());
	}

	int ShaderProgram::GetUniformLocation(const std::string& uni)
	{
		return glGetUniformLocation(progID, uni.c_str());
	}

	void ShaderProgram::Bind()
	{
		if (currentProgram != this)
		{
			glUseProgram(progID);
			currentProgram = this;
		}
	}

	void ShaderProgram::Unbind()
	{
		glUseProgram(0);
		currentProgram = NULL;
	}

	ShaderProgram* ShaderProgram::CurrentProgram()
	{
		if (currentProgram == NULL)
			FatalError(string("ShaderProgram::CurrentProgram - There is no active shader program"))

		return currentProgram;
	}
}