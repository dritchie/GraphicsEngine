#include "Assets/Material/SmoothDiffGeo.h"


namespace GraphicsEngine
{
	void SmoothDiffGeo::ShaderCode(ShaderAssetCode& code)
	{
		code.AddVertCode(Define("PLUMB_NORMALS", ""));
		code.AddFragCode(Define("PLUMB_NORMALS", ""));
		code.AddFragCode(Block("DIFF_GEO", "#include \"Smooth.DiffGeo\""));
	}

	void SmoothDiffGeo::Bind()
	{
		// This does nothing
	}
}