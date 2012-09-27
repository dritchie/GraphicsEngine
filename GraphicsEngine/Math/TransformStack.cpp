#include "Math/TransformStack.h"
#include <stack>
#include "Common/GL.h"
#include "Assets/Shader/ShaderProgram.h"


using std::string;


namespace GraphicsEngine
{
	TransformStack TransformStack::modelview = TransformStack("ModelViewMatrix", "NormalMatrix");
	TransformStack TransformStack::projection = TransformStack("ProjectionMatrix");

	TransformStack::TransformStack(const string& name, const string& invTransName)
		: _name(name), _invTransName(invTransName)
	{
		theStack.push(Transform::Identity());
		RefreshFields();
	}

	void TransformStack::LoadIdentity()
	{
		theStack.top() = Transform::Identity();
		RefreshFields();
	}

	void TransformStack::Load(const Transform& t)
	{
		theStack.top() = t;
		RefreshFields();
	}

	void TransformStack::Push()
	{
		theStack.push(theStack.top());
	}

	void TransformStack::Pop()
	{
		if (theStack.size() <= 1)
			FatalError(string("TransformStack::Pop() - Stack is empty!"))
		else
		{
			theStack.pop();
			RefreshFields();
		}
	}

	void TransformStack::Multiply(const Transform& t)
	{
		Transform& b = theStack.top();
		b *= t;
		RefreshFields();
	}

	void TransformStack::RefreshFields()
	{
		top = theStack.top();
		topInv = top.Inverse();
		topInvTrans = topInv.Transpose();
	}

	const Transform& TransformStack::Top() const
	{
		return top;
	}

	const Transform& TransformStack::TopInverse() const
	{
		return topInv;
	}

	const Transform& TransformStack::TopInverseTranspose() const
	{
		return topInvTrans;
	}

	TransformStack& TransformStack::Modelview()
	{
		return modelview;
	}

	TransformStack& TransformStack::Projection()
	{
		return projection;
	}

	void TransformStack::Bind() const
	{
		ShaderProgram* prog = ShaderProgram::CurrentProgram();

		int loc = prog->GetUniformLocation(_name);
		if (loc >= 0)
			glUniformMatrix4fv(loc, 1, GL_FALSE, Top().GetGLMatrix());
		if (!_invTransName.empty())
		{
			int invTransLoc = prog->GetUniformLocation(_invTransName);
			if (invTransLoc >= 0)
				glUniformMatrix4fv(invTransLoc, 1, GL_FALSE, TopInverseTranspose().GetGLMatrix());
		}
	}
}