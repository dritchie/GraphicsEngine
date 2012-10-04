#include "Viewing/Camera.h"
#include "Common/GL.h"
#include "Math/Transform.h"
#include "Eigen/Geometry"


using namespace Eigen;


namespace GraphicsEngine
{
	Camera::Camera()
		: eyePos(0.0f,0.0f,0.0f), lookAtPoint(0.0f,0.0f,-1.0f), lookVec(0.0f,0.0f,-1.0f),
		  upVec(0.0f,1.0f,0.0f), leftVec(-1.0f,0.0f,0.0f), absUp(0.0f,1.0f,0.0f)
	{
	}

	Camera::Camera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up, const Vector3f& worldUp)
		: absUp(worldUp)
	{
		Reset(eye, lookAt, up);
	}

	void Camera::Reset(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up)
	{
		eyePos = eye;
		lookAtPoint = lookAt;
		upVec = up.normalized();
		lookVec = (lookAtPoint - eyePos).normalized();
		leftVec = upVec.cross(lookVec);
	}

	Transform Camera::GetLookAtTransform()
	{
		return Transform::LookAt(eyePos, lookAtPoint, upVec);
	}

	void Camera::DollyLeft(float dist)
	{
		Vector3f offset = dist*leftVec;
		eyePos += offset;
		lookAtPoint += offset;
	}

	void Camera::DollyForward(float dist)
	{
		Vector3f offset = dist*lookVec;
		eyePos += offset;
		lookAtPoint += offset;
	}

	void Camera::DollyUp(float dist)
	{
		Vector3f offset = dist*upVec;
		eyePos += offset;
		lookAtPoint += offset;
	}

	void Camera::Zoom(float dist)
	{
		Vector3f offset = dist*lookVec;
		eyePos += offset;
	}

	void Camera::PanLeft(float theta)
	{
		Transform t = Transform::Rotation(absUp, theta, eyePos);
		Vector3f lookdir = lookAtPoint - eyePos;
		lookdir = t.TransformVector(lookdir);
		lookAtPoint = eyePos + lookdir;
		lookVec = lookdir.normalized();
		upVec = t.TransformVector(upVec);
		leftVec = t.TransformVector(leftVec);
	}

	void Camera::PanUp(float theta)
	{
		Transform t = Transform::Rotation(leftVec, theta, eyePos);
		Vector3f lookdir = lookAtPoint - eyePos;
		lookdir = t.TransformVector(lookdir);
		lookAtPoint = eyePos + lookdir;
		lookVec = lookdir.normalized();
		upVec = t.TransformVector(upVec);
	}

	void Camera::OrbitLeft(float theta)
	{
		Transform t = Transform::Rotation(absUp, theta, lookAtPoint);
		Vector3f invLookdir = eyePos - lookAtPoint;
		invLookdir = t.TransformVector(invLookdir);
		eyePos = lookAtPoint + invLookdir;
		lookVec = -invLookdir.normalized();
		upVec = t.TransformVector(upVec);
		leftVec = t.TransformVector(leftVec);
	}

	void Camera::OrbitUp(float theta)
	{
		Transform t = Transform::Rotation(leftVec, theta, lookAtPoint);
		Vector3f invLookdir = eyePos - lookAtPoint;
		invLookdir = t.TransformVector(invLookdir);
		eyePos = lookAtPoint + invLookdir;
		lookVec = -invLookdir.normalized();
		upVec = t.TransformVector(upVec);
	}

	void Camera::TrackTo(const Eigen::Vector3f& newPos)
	{
		Reset(newPos, lookAtPoint, upVec);
	}
}