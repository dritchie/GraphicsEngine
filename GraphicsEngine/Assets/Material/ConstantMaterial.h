#pragma once

#ifndef GRAPHICS_ENGINE__CONSTANT_MATERIAL_H
#define GRAPHICS_ENGINE__CONSTANT_MATERIAL_H

#include "Assets/Material/Material.h"
#include "Eigen/Core"

namespace GraphicsEngine
{
	/** Constant materials just shade the surface using a constant color **/
	class ConstantMaterial : public Material
	{
	public:

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		ConstantMaterial();

		void ShaderCode(ShaderAssetCode& code);
		void Bind();

		Eigen::Vector4f color;
	};
}

#endif