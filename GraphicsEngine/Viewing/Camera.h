#pragma once

#ifndef GRAPHICSENGINE__CAMERA_H
#define GRAPHICSENGINE__CAMERA_H


#include "Common/Common.h"
#include "Application/Keys.h"
#include "Eigen/Core"
#include "Math/Transform.h"


namespace GraphicsEngine
{

	/**
	Base class for all virtual cameras
	**/
	class Camera
	{
	public:

		Camera();
		Camera(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up, const Eigen::Vector3f& worldUp = Eigen::Vector3f::UnitY());

		void Serialize(std::ostream& stream);
		void Deserialize(std::istream& stream);

		Transform GetLookAtTransform() const;

		// All the low-level controls
		void Reset(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up);
		void DollyLeft(float dist);
		void DollyForward(float dist);
		void DollyUp(float dist);
		void Zoom(float dist);
		void PanLeft(float theta);
		void PanUp(float theta);
		void OrbitLeft(float theta);
		void OrbitUp(float theta);
		void TrackTo(const Eigen::Vector3f& newPos);

	protected:
		Eigen::Vector3f eyePos, lookAtPoint, lookVec, upVec, leftVec, absUp;
	};


	/**
	Base class for all user-controllable virtual cameras
	**/
	class InteractiveCamera : public Camera
	{
	public:

		InteractiveCamera() : Camera() {}
		InteractiveCamera(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up, const Eigen::Vector3f& worldUp = Eigen::Vector3f::UnitY())
			: Camera(eye,lookAt,up, worldUp) {}

		// Event handlers return 'true' if the application needs to re-render
		// as a consequence of their actions.

		// Keyboard events
		virtual bool KeyDown(int key, const ModifierKeys& mods) { return false; }
		virtual bool KeyUp(int key, const ModifierKeys& mods) { return false; }

		// Mouse events
		virtual bool MouseDown(int button, int x, int y, const ModifierKeys& mods) { return false; }
		virtual bool MouseUp(int button, int x, int y, const ModifierKeys& mods) { return false; }
		virtual bool MouseMove(int x, int y, const ModifierKeys& mods) { return false; }
		virtual bool MouseDrag(int button, int x, int y, const ModifierKeys& mods) { return false; }
		virtual bool MouseWheel(int delta, const ModifierKeys& mods) { return false; }

		virtual bool Timestep(float dt) { return false; }
	};

}


#endif