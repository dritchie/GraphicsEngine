#include "Assets/Material/PhongBSDF.h"
#include "Assets/UniformSources.h"

using namespace std;

namespace GraphicsEngine
{
	PhongBSDF::PhongBSDF(UniformVec4Source* _ka,
						 UniformVec4Source* _kd,
						 UniformVec4Source* _ks,
						 UniformFloatSource* _shine)
						 : ka(_ka), kd(_kd), ks(_ks), shine(_shine)
	{
	}

	PhongBSDF::~PhongBSDF()
	{
		SAFEDELETE(ka);
		SAFEDELETE(kd);
		SAFEDELETE(ks);
		SAFEDELETE(shine);
	}

	void PhongBSDF::ShaderCode(ShaderAssetCode& code)
	{
		// Get definitions from each source component
		ka->name = "PHONG_KA"; ka->ShaderCode(code);
		kd->name = "PHONG_KD"; kd->ShaderCode(code);
		ks->name = "PHONG_KS"; ks->ShaderCode(code);
		shine->name = "PHONG_SHINE"; shine->ShaderCode(code);

		// Finally, we need to define the overall BSDF block
		code.AddFragCode(Block("BSDF", "#include \"Phong.BSDF\""));
	}

	void PhongBSDF::Bind()
	{
		ka->name = "PHONG_KA"; ka->Bind();
		kd->name = "PHONG_KD"; kd->Bind();
		ks->name = "PHONG_KS"; ks->Bind();
		shine->name = "PHONG_SHINE"; shine->Bind();
	}
}