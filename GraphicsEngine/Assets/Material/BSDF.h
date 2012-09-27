#pragma once

#ifndef GRAPHICS_ENGINE__CONSTANT_BRDF_H
#define GRAPHICS_ENGINE__CONSTANT_BRDF_H

#include "Assets/ShaderAsset.h"


namespace GraphicsEngine
{
	/** Abstract base class for all BSDFs **/
	class BSDF : public ShaderAsset
	{
	};
}


#endif