#include "Assets/Material/ScatteringMaterial.h"


namespace GraphicsEngine
{
	ScatteringMaterial::ScatteringMaterial(BSDF* b, DiffGeo* dg)
		: bsdf(b), diffGeo(dg)
	{
	}

	ScatteringMaterial::~ScatteringMaterial()
	{
		delete bsdf;
		delete diffGeo;
	}

	void ScatteringMaterial::ShaderCode(ShaderAssetCode& code)
	{
		code.AddFragCode(Block("MATERIAL", "#include \"Scattering.Material\""));

		bsdf->ShaderCode(code);
		diffGeo->ShaderCode(code);
	}

	void ScatteringMaterial::Bind()
	{
		bsdf->Bind();
		diffGeo->Bind();
	}

	bool ScatteringMaterial::CodeChanged()
	{
		return bsdf->CodeChanged() || diffGeo->CodeChanged();
	}
}