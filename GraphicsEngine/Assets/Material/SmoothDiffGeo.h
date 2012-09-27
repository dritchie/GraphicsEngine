#pragma once

#ifndef GRAPHICS_ENGINE__SMOOTH_DIFF_GEO_H
#define GRAPHICS_ENGINE__SMOOTH_DIFF_GEO_H

#include "Assets/Material/DiffGeo.h"

namespace GraphicsEngine
{
	/** Smooth differential geometry just uses the raw surface normal for shading **/
	class SmoothDiffGeo : public DiffGeo
	{
	public:

		void ShaderCode(ShaderAssetCode& code);
		void Bind();
	};
}

#endif