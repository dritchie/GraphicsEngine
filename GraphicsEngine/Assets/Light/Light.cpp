#include "Assets/Light/Light.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Common/GL.h"
#include "Math/Math.h"

using Eigen::Vector3f;
using namespace std;

namespace GraphicsEngine
{
	Light::Light()
		: lightType(PointLight),
		  pos(0, 0, 0),
		  dir(0, 0, 0),
		  col(0, 0, 0, 0),
		  spotExp(0),
		  spotCutoff(180),
		  attenConst(1),
		  attenLin(0),
		  attenQuad(0)
	{
	}

	void Light::Bind(const string& name, const Transform& xform) const
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();

		int typeLoc = prog->GetUniformLocation(name + string(".lightType"));
		int posLoc = prog->GetUniformLocation(name + string(".pos"));
		int dirLoc = prog->GetUniformLocation(name + string(".dir"));
		int colLoc = prog->GetUniformLocation(name + string(".col"));
		int spotExpLoc = prog->GetUniformLocation(name + string(".spotExp"));
		int spotCutLoc = prog->GetUniformLocation(name + string(".spotCutoff"));
		int attenConstLoc = prog->GetUniformLocation(name + string(".attenConst"));
		int attenLinLoc = prog->GetUniformLocation(name + string(".attenLin"));
		int attenQuadLoc = prog->GetUniformLocation(name + string(".attenQuad"));

		if (typeLoc >= 0)
			glUniform1i(typeLoc, lightType);
		if (posLoc >= 0)
		{
			Vector3f xpos = xform.TransformPoint(pos);
			glUniform3fv(posLoc, 1, xpos.data());
		}
		if (dirLoc >= 0)
		{
			Vector3f xdir = xform.TransformVector(dir.normalized());
			glUniform3fv(dirLoc, 1, xdir.data());
		}
		if (colLoc >= 0)
			glUniform4fv(colLoc, 1, col.data());
		if (spotExpLoc >= 0)
			glUniform1f(spotExpLoc, spotExp);
		if (spotCutLoc >= 0)
			glUniform1f(spotCutLoc, cosf(DegreesToRadians(spotCutoff)));
		if (attenConstLoc >= 0)
			glUniform1f(attenConstLoc, attenConst);
		if (attenLinLoc >= 0)
			glUniform1f(attenLinLoc, attenLin);
		if (attenQuadLoc >= 0)
			glUniform1f(attenQuadLoc, attenQuad);
	}
}