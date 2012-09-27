#include "Math/Math.h"
#include "Math/Transform.h"
#include "Eigen/LU"
#include "aiVector3D.h"
#include "aiMatrix4x4.h"

using namespace Eigen;


namespace GraphicsEngine
{
	Transform::Transform()
	{
		m = Matrix4f::Zero();
	}

	Transform::Transform(const Matrix4f& mat)
		: m(mat)
	{
	}

	Transform::Transform(const aiMatrix4x4& mat)
	{
		m << mat.a1, mat.a2, mat.a3, mat.a4,
			 mat.b1, mat.b2, mat.b3, mat.b4,
			 mat.c1, mat.c2, mat.c3, mat.c4,
			 mat.d1, mat.d2, mat.d3, mat.d4;
	}

	Transform Transform::Identity()
	{
		Transform t;
		t.m = Matrix4f::Identity();
		return t;
	}

	Transform Transform::Translation(const Vector3f& t)
	{
		return Translation(t.x(), t.y(), t.z());
	}

	Transform Transform::Translation(float tx, float ty, float tz)
	{
		Transform t = Identity();
		t.m(0,3) = tx;
		t.m(1,3) = ty;
		t.m(2,3) = tz;
		return t;
	}

	Transform Transform::RotationX(float r)
	{
		float cosr = cosf(r);
		float sinr = sinf(r);

		Matrix4f t = Matrix4f::Identity();
		t(1,1) = cosr;
		t(1,2) = -sinr;
		t(2,1) = sinr;
		t(2,2) = cosr;
		return t;
	}

	Transform Transform::RotationY(float r)
	{
		float cosr = cosf(r);
		float sinr = sinf(r);

		Matrix4f t = Matrix4f::Identity();
		t(0,0) = cosr;
		t(2,0) = -sinr;
		t(0,2) = sinr;
		t(2,2) = cosr;
		return t;
	}

	Transform Transform::RotationZ(float r)
	{
		float cosr = cosf(r);
		float sinr = sinf(r);

		Matrix4f t = Matrix4f::Identity();
		t(0,0) = cosr;
		t(0,1) = -sinr;
		t(1,0) = sinr;
		t(1,1) = cosr;
		return t;
	}

	Transform Transform::Rotation(const Vector3f& axis, float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		float t = 1.0f - c;

		Vector3f normalizedAxis = axis.normalized();
		float x = normalizedAxis.x();
		float y = normalizedAxis.y();
		float z = normalizedAxis.z();

		Matrix4f result;

		result(0,0) = 1 + t*(x*x-1);
		result(1,0) = z*s+t*x*y;
		result(2,0) = -y*s+t*x*z;
		result(3,0) = 0.0f;

		result(0,1) = -z*s+t*x*y;
		result(1,1) = 1+t*(y*y-1);
		result(2,1) = x*s+t*y*z;
		result(3,1) = 0.0f;

		result(0,2) = y*s+t*x*z;
		result(1,2) = -x*s+t*y*z;
		result(2,2) = 1+t*(z*z-1);
		result(3,2) = 0.0f;

		result(0,3) = 0.0f;
		result(1,3) = 0.0f;
		result(2,3) = 0.0f;
		result(3,3) = 1.0f;

		return result;
	}

	Transform Transform::Rotation(const Vector3f& axis, float angle, const Vector3f& center)
	{
		return Translation(center) * Rotation(axis, angle) * Translation(-center);
	}

	Transform Transform::Scale(float s)
	{
		return Scale(s,s,s);
	}

	Transform Transform::Scale(float sx, float sy, float sz)
	{
		Transform t;
		t.m(0,0) = sx;
		t.m(1,1) = sy;
		t.m(2,2) = sz;
		t.m(3,3) = 1.0f;
		return t;
	}

	Transform Transform::LookAt(const Vector3f& eyePos, const Vector3f& lookAtPoint, const Vector3f& upVec)
	{
		Vector3f w = (eyePos - lookAtPoint).normalized();
		Vector3f u = upVec.cross(w).normalized();
		Vector3f v = w.cross(u);

		Matrix4f rot;
		rot << u.x(), u.y(), u.z(), 0,
			   v.x(), v.y(), v.z(), 0,
			   w.x(), w.y(), w.z(), 0,
			   0,     0,     0,     1;

		return Transform(rot) * Translation(-eyePos);
	}

	Transform Transform::Ortho(float top, float bottom, float left, float right, float near, float far)
	{
		Matrix4f mat;
		mat << 2.0f / (right - left), 0, 0, -(right+left)/(right-left),
			   0, 2.0f / (top - bottom), 0, -(top+bottom)/(top-bottom),
			   0, 0, -2.0f / (far - near),  -(far+near)/(far-near),
			   0, 0, 0, 1;
		return Transform(mat);
	}

    Transform Transform::Perspective(float fovy, float aspect, float near, float far)
	{
		float f = 1.0f / tanf(DegreesToRadians(fovy)/2.0f);
		Matrix4f mat;
		mat << f / aspect, 0, 0, 0,
			   0, f, 0, 0,
			   0, 0, (far+near)/(near-far), (2*far*near)/(near-far),
			   0, 0, -1, 0;
		return Transform(mat);
	}

	Transform Transform::Inverse() const
	{
		Transform t = *this;
		t.m = t.m.inverse().eval();
		return t;
	}

	Transform Transform::Transpose() const
	{
		Transform t = *this;
		t.m = t.m.transpose().eval();
		return t;
	}

	float Transform::Determinant() const
	{
		return m.determinant();
	}

}