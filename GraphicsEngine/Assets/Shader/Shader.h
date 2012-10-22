#pragma once

#ifndef GRAPHICS_ENGINE__SHADER_H
#define GRAPHICS_ENGINE__SHADER_H

#include "Common/Common.h"

namespace GraphicsEngine
{
	/** 
	Represents a compiled shader for one stage
	of the graphics pipeline
	**/
	class Shader
	{
	public:

		enum ShaderStage
		{
			VertexShader = 0,
			TessellationControlShader,
			TessellationEvalShader,
			GeometryShader,
			FragmentShader
		};

		Shader(ShaderStage sstage);
		~Shader();

		void FromFile(const std::string& filename);
		void FromSource(const std::string& sourceCode);

		__forceinline ShaderStage Stage() const { return stage; }

		static const std::string& ShaderPath();
		static void ShaderPath(const std::string& path);
		static std::string LoadSource(const std::string& filename);

		friend class ShaderProgram;

	private:

		void FromParsedSource(const std::string& src);
		void ConfirmCompile();

		UINT shaderID;
		ShaderStage stage;

		static std::string shaderPath;
	};

	__forceinline const std::string& Shader::ShaderPath()
	{
		return shaderPath;
	}

	__forceinline void Shader::ShaderPath(const std::string& path)
	{
		shaderPath = RStrip(RStrip(path, '/'), '\\') + "/";
	}
}


#endif