#pragma once

#ifndef GRAPHICS_ENGINE__SCATTERING_MATERIAL_H
#define GRAPHICS_ENGINE__SCATTERING_MATERIAL_H

#include "Assets/Material/Material.h"
#include "Assets/Material/BSDF.h"
#include "Assets/Material/DiffGeo.h"

namespace GraphicsEngine
{
	/**
	Scattering materials are materials that interact with light.
	They are composed of a BSDF and a model of surface differential geometry.
	**/
	class ScatteringMaterial : public Material
	{
	public:

		ScatteringMaterial(BSDF* b, DiffGeo* dg);
		~ScatteringMaterial();

		void ShaderCode(ShaderAssetCode& code);
		void Bind();
		bool CodeChanged();

	private:

		BSDF* bsdf;
		DiffGeo* diffGeo;
	};
}

#endif