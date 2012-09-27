#pragma once

#ifndef GRAPHICS_ENGINE__MULTI_LIGHT_SYSTEM_H
#define GRAPHICS_ENGINE__MULTI_LIGHT_SYSTEM_H

#include "Assets/Light/LightSystem.h"
#include "Math/Transform.h"
#include "Eigen/Core"
#include <deque>

namespace GraphicsEngine
{
	class Light;

	/** Light system for rendering with multiple lights in one pass **/
	class MultiLightSystem : public LightSystem
	{
	public:

		MultiLightSystem();

		void Push(Light* light, const Transform& xform);
		void Pop();
		void Clear();

		void ShaderCode(ShaderAssetCode& code);
		void Bind();
		bool CodeChanged();

	private:

		typedef std::pair<Light*, Transform> StackEntry;
		std::deque< StackEntry, Eigen::aligned_allocator< StackEntry > > theStack;

		int lastMaxNumLights;
	};
}

#endif