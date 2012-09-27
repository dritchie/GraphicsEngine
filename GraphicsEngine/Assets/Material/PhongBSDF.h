#pragma once

#ifndef GRAPHICS_ENGINE__CONSTANT_BSDF_H
#define GRAPHICS_ENGINE__CONSTANT_BSDF_H

#include "Assets/Material/BSDF.h"

namespace GraphicsEngine
{
	class UniformVec4Source;
	class UniformFloatSource;

	/** A 'BSDF' based on the Phong illumination model **/
	class PhongBSDF : public BSDF
	{
	public:

		PhongBSDF(UniformVec4Source* _ka,
		          UniformVec4Source* _kd,
				  UniformVec4Source* _ks,
				  UniformFloatSource* _shine);
		~PhongBSDF();

		void ShaderCode(ShaderAssetCode& code);
		void Bind();

	private:

		UniformVec4Source* ka;
		UniformVec4Source* kd;
		UniformVec4Source* ks;
		UniformFloatSource* shine;
	};
}

#endif