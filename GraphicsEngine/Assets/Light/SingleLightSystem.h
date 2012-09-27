#pragma once

#ifndef GRAPHICS_ENGINE__SINGLE_LIGHT_SYSTEM_H
#define GRAPHICS_ENGINE__SINGLE_LIGHT_SYSTEM_H

#include "Assets/Light/LightSystem.h"
#include "Math/Transform.h"

namespace GraphicsEngine
{
	class Light;

	/** Light system that contains just one light **/
	class SingleLightSystem : public LightSystem
	{
	public:

		SingleLightSystem(Light* l, const Transform& t);

		void ShaderCode(ShaderAssetCode& code);
		void Bind();

	private:

		Light* light;
		Transform xform;
	};
}

#endif