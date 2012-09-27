#pragma once

#ifndef GRAPHICSENGINE__LIGHT_H
#define GRAPHICSENGINE__LIGHT_H

#include "Common/Common.h"
#include "Eigen/Core"
#include "Math/Transform.h"

namespace GraphicsEngine
{
	class Light
	{
	public:

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		
		enum LightType
		{
			PointLight = 0,
			DirectionalLight
		};

		Light();

		void Bind(const std::string& name, const Transform& xform = Transform::Identity()) const;

		LightType lightType;
		Eigen::Vector3f pos;
		Eigen::Vector3f dir;
		Eigen::Vector4f col;
		float spotExp;
		float spotCutoff;
		float attenConst;
		float attenLin;
		float attenQuad;
	};
}


#endif