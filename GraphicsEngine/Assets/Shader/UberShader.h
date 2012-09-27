#pragma once

#ifndef GRAPHICS_ENGINE__UBER_SHADER_H
#define GRAPHICS_ENGINE__UBER_SHADER_H

#include "Assets/ShaderAsset.h"
#include <unordered_set>
#include <unordered_map>

namespace GraphicsEngine
{
	class Shader;
	class ShaderProgram;

	/**
	An uber shader is a complete collection of shaders (for all pipeline stages)
	composed of the union of all desired shader functionality. By passing in extra
	code, the uber shader can be conditionally compiled into a program that meets
	specific needs
	**/
	class UberShader
	{
	public:

		UberShader(const std::string& vertFile,
			       const std::string& fragFile,
				   const std::string& geoFile = "",
				   const std::string& tessControlFile = "",
				   const std::string& tessEvalFile = "");
		~UberShader();

		// Get the program defined by the provided definitions. May invoke
		// conditional compilation if the program does not already exist.
		ShaderProgram* GetProgram(const ShaderAssetCode& code);

		// Delete this program from the internal cache. Useful when the client
		// application knows that there are no processes that depend on this program any more.
		void DeleteProgram(ShaderProgram* prog);

		// Completely clear the internal cache of compiled shaders/programs.
		void ClearCache();

	private:

		ShaderProgram* Compile(const ShaderAssetCode& code);
		void CheckedDeleteShader(Shader* shader);

		std::string vertSrc, fragSrc, geoSrc, tcSrc, teSrc;

		std::unordered_map<Shader*, std::unordered_set<ShaderProgram*> > shader2programs;
		std::unordered_map<std::string, Shader*> code2shader;
		std::unordered_map<std::string, ShaderProgram*> code2program;
	};
}

#endif