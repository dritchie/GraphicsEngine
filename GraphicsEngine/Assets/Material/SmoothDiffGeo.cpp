#include "Assets/Material/SmoothDiffGeo.h"


namespace GraphicsEngine
{
	void SmoothDiffGeo::ShaderCode(ShaderAssetCode& code)
	{
		code.vertCode.push_back(Define("PLUMB_NORMALS", ""));
		code.fragCode.push_back(Define("PLUMB_NORMALS", ""));
		code.fragCode.push_back(Block("DIFF_GEO", "#include \"Smooth.DiffGeo\""));
	}

	void SmoothDiffGeo::Bind()
	{
		// This does nothing
	}
}