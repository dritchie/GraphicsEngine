#pragma once

#ifndef GRAPHICSENGINE__TRANSFORM_H
#define GRAPHICSENGINE__TRANSFORM_H

#include "Eigen/Core"

struct aiMatrix4x4;

namespace GraphicsEngine
{
	/**
	Convenience wrapper for Eigen::Matrix4f that provides common
	affine transformation functions

	(Some code borrowed from Matt Fisher)
	**/
	class Transform
	{
	public:

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		Transform();
		Transform(const Eigen::Matrix4f& mat);
		Transform(const aiMatrix4x4& mat);

		__forceinline const Eigen::Matrix4f& GetMatrix() const { return m; }
		__forceinline Eigen::Matrix4f& GetMatrix() { return m; }

		// Generators
		static Transform Identity();
		static Transform Translation(const Eigen::Vector3f& t);
		static Transform Translation(float tx, float ty, float tz);
		static Transform RotationX(float r);
		static Transform RotationY(float r);
		static Transform RotationZ(float r);
		static Transform Rotation(const Eigen::Vector3f& axis, float angle);
		static Transform Rotation(const Eigen::Vector3f& axis, float angle, const Eigen::Vector3f& center);
		static Transform Scale(float s);
		static Transform Scale(float sx, float sy, float sz);
		static Transform LookAt(const Eigen::Vector3f& eyePos, const Eigen::Vector3f& lookAtPoint, const Eigen::Vector3f& upVec);
		static Transform Ortho(float top, float bottom, float left, float right, float near, float far);
		static Transform Perspective(float fovy, float aspect, float near, float far);

		// Operators
		friend Transform operator * (const Transform& t1, const Transform& t2);			// Composition
		Transform& operator *= (const Transform& t);									// Composition (in-place)
		friend Transform operator * (float s, const Transform& t);						// Scalar multiplcation
		friend Transform operator * (const Transform& t, float s);						// Scalar multiplcation
		Transform& operator *= (float s);												// Scalar multiplcation (in-place)

		// Application
		Eigen::Vector3f TransformPoint(const Eigen::Vector3f& point) const;
		Eigen::Vector3f TransformVector(const Eigen::Vector3f& vec) const;

		// Misc wrappers
		Transform Inverse() const;
		Transform Transpose() const;
		float Determinant() const;

		// Converting to OpenGL matrix format
		const float* GetGLMatrix() const;

		// For debugging
		friend std::ostream& operator << (std::ostream& stream, const Transform& t);

	private:
		Eigen::Matrix4f m;
	};


	__forceinline Eigen::Vector3f Transform::TransformPoint(const Eigen::Vector3f& point) const
	{
		float w = point.x() * m(3,0) + point.y() * m(3,1) + point.z() * m(3,2) + m(3,3);
        if(w)
        {
            const float invW = 1.0f / w;
            return Eigen::Vector3f( (point.x() * m(0,0) + point.y() * m(0,1) + point.z() * m(0,2) + m(0,3)) * invW,
                                    (point.x() * m(1,0) + point.y() * m(1,1) + point.z() * m(1,2) + m(1,3)) * invW,
                                    (point.x() * m(2,0) + point.y() * m(2,1) + point.z() * m(2,2) + m(2,3)) * invW);
        }
        else
        {
			return Eigen::Vector3f::Zero();
        }
	}

	__forceinline Eigen::Vector3f Transform::TransformVector(const Eigen::Vector3f& vec) const
	{
		return Eigen::Vector3f(vec.x() * m(0,0) + vec.y() *  m(0,1) + vec.z() * m(0,2),
                               vec.x() * m(1,0) + vec.y() *  m(1,1) + vec.z() * m(1,2),
                               vec.x() * m(2,0) + vec.y() *  m(2,1) + vec.z() * m(2,2));
	}

	__forceinline Transform operator * (const Transform& t1, const Transform& t2)
	{
		Transform t = t1;
		t *= t2;
		return t;
	}

	__forceinline Transform& Transform::operator *= (const Transform& t)
	{
		m *= t.m;
		return *this;
	}

	__forceinline Transform operator * (float s, const Transform& t)
	{
		Transform tt = t;
		tt *= s;
		return tt;
	}

	__forceinline Transform operator * (const Transform& t, float s)
	{
		return s * t;
	}

	__forceinline Transform& Transform::operator *= (float s)
	{
		m *= s;
		return *this;
	}

	__forceinline const float* Transform::GetGLMatrix() const
	{
		return m.data();
	}

	__forceinline std::ostream& operator << (std::ostream& stream, const Transform& t)
	{
		return stream << t.m;
	}
}


#endif