#pragma once

#ifndef GRAPHICSENGINE__SHADER_PROGRAM_H
#define GRAPHICSENGINE__SHADER_PROGRAM_H


#include "Common/Common.h"
#include <unordered_map>


namespace GraphicsEngine
{
	class Shader;

	/**
	Encapsulates a complete shader program over 
	multiple pipeline stages
	**/
	class ShaderProgram
	{
	public:
		
		ShaderProgram(const std::vector<Shader*>& componentShaders);
		~ShaderProgram();

		void Bind();
		void Unbind();

		int GetAttributeLocation(const std::string& attrib);
		int GetUniformLocation(const std::string& uni);

		static ShaderProgram* CurrentProgram();

		friend class UberShader;

	private:

		void ConfirmLink();

		UINT progID;
		std::vector<Shader*> shaders;

		static ShaderProgram* currentProgram;
	};
}


#endif