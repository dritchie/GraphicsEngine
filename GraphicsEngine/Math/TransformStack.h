#pragma once

#ifndef GRAPHICSENGINE__TRANSFORM_STACK_H
#define GRAPHICSENGINE__TRANSFORM_STACK_H


#include "Common/Common.h"
#include "Math/Transform.h"
#include "Eigen/Core"
#include <stack>


namespace GraphicsEngine
{
	class ShaderProgram;

	/**
	A stack of transformations (useful for transformation hierarchies)
	**/
	class TransformStack
	{
	public:

		TransformStack(const std::string& name, const std::string& invTransName = "");
		void LoadIdentity();
		void Load(const Transform& t);
		void Push();
		void Pop();
		void Multiply(const Transform& t);
		
		const Transform& Top() const;
		const Transform& TopInverse() const;
		const Transform& TopInverseTranspose() const;

		void Bind() const;

		static TransformStack& Modelview();
		static TransformStack& Projection();

	private:

		void RefreshFields();

		typedef std::stack<Transform, std::deque<Transform, Eigen::aligned_allocator<Transform> > > StackType;
		StackType theStack;
		Transform top;
		Transform topInv;
		Transform topInvTrans;

		std::string _name;
		std::string _invTransName;

		static TransformStack modelview;
		static TransformStack projection;
	};
}


#endif