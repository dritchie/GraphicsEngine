#include "Assets/Light/SingleLightSystem.h"
#include "Assets/Light/Light.h"


namespace GraphicsEngine
{
	SingleLightSystem::SingleLightSystem(Light* l, const Transform& t)
		: light(l), xform(t)
	{
	}

	void SingleLightSystem::ShaderCode(ShaderAssetCode& code)
	{
		code.AddFragCode(Block("LIGHT_SYSTEM", "#include \"Single.LightSystem\""));
	}

	void SingleLightSystem::Bind()
	{
		light->Bind("light", xform);
	}
};