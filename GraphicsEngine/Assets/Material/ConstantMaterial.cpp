#include "Assets/Material/ConstantMaterial.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Common/GL.h"

using namespace std;

namespace GraphicsEngine
{
	ConstantMaterial::ConstantMaterial()
		: color(0, 0, 0, 0)
	{
	}

	void ConstantMaterial::ShaderCode(ShaderAssetCode& code)
	{
		code.AddFragCode(Block("MATERIAL", "#include \"Constant.Material\""));
	}

	void ConstantMaterial::Bind()
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();
		int colLoc = prog->GetUniformLocation("constant_material_color");
		if (colLoc >= 0)
			glUniform4fv(colLoc, 1, color.data());
	}
}