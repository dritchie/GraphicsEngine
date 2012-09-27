#pragma once

#ifndef GRAPHICS_ENGINE__UNIFORM_SOURCES_H
#define GRAPHICS_ENGINE__UNIFORM_SOURCES_H

#include "Assets/ShaderAsset.h"
#include "Eigen/Core"

namespace GraphicsEngine
{
	/**
	Uniform sources are sources of shader 'uniform' data.
	They provide an '_Evaluate()' method that returns their value
	(This value may be a contant, or come from a texture, or be
	procedurally generated, etc.)
	**/

	class UniformVec4Source : public ShaderAsset
	{
	public:
		std::string name;

	protected:
		std::string WrapInEvalFunction(const std::string& code);
	};

	class UniformFloatSource : public ShaderAsset
	{
	public:
		std::string name;

	protected:
		std::string WrapInEvalFunction(const std::string& code);
	};

	class ConstantVec4Source : public UniformVec4Source
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		ConstantVec4Source(const Eigen::Vector4f& v = Eigen::Vector4f(0.0f,0.0f,0.0f,0.0f))
		: vec4(v) {}
		void ShaderCode(ShaderAssetCode& code);
		void Bind();
		Eigen::Vector4f vec4;
	};

	class ConstantFloatSource : public UniformFloatSource
	{
	public:
		ConstantFloatSource(float f = 0.0f) : fl(f) {}
		void ShaderCode(ShaderAssetCode& code);
		void Bind();
		float fl;
	};

	class Texture2D;

	class TextureVec4Source : public UniformVec4Source
	{
	public:
		TextureVec4Source(Texture2D* t = NULL) : tex(t) {}
		void ShaderCode(ShaderAssetCode& code);
		void Bind();
		Texture2D* tex;
	};

	class TextureFloatSource : public UniformFloatSource
	{
	public:
		TextureFloatSource(Texture2D* t = NULL) : tex(t) {}
		void ShaderCode(ShaderAssetCode& code);
		void Bind();
		Texture2D* tex;
	};

}

#endif