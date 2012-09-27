#pragma once

#ifndef GRAPHICSENGINE__FPS_CAMERA_H
#define GRAPHICSENGINE__FPS_CAMERA_H


#include "Viewing/Camera.h"


namespace GraphicsEngine
{
	/**
	FPS-style camera (WASD + mouse)
	**/
	class FPSCamera : public InteractiveCamera
	{
	public:

		FPSCamera();
		FPSCamera(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up);

		// Accessors
		const float& MoveSpeed() const;
		float& MoveSpeed();
		const float& LookSpeed() const;
		float& LookSpeed();
		const bool& InvertY() const;
		bool& InvertY();

		// Keyboard events
		bool KeyDown(int key, const ModifierKeys& mods);
		bool KeyUp(int key, const ModifierKeys& mods);

		// Mouse events
		bool MouseDown(int button, int x, int y, const ModifierKeys& mods);
		bool MouseDrag(int button, int x, int y, const ModifierKeys& mods);

		bool Timestep(float dt);

	private:
		bool movingForward, movingBackward, movingLeft, movingRight;
		float moveSpeed, lookSpeed;
		int prevx, prevy;
		bool invertY;
	};


	__forceinline const float& FPSCamera::MoveSpeed() const
	{
		return moveSpeed;
	}

	__forceinline float& FPSCamera::MoveSpeed()
	{
		return moveSpeed;
	}

	__forceinline const float& FPSCamera::LookSpeed() const
	{
		return lookSpeed;
	}

	__forceinline float& FPSCamera::LookSpeed()
	{
		return lookSpeed;
	}

	__forceinline const bool& FPSCamera::InvertY() const
	{
		return invertY;
	}

	__forceinline bool& FPSCamera::InvertY()
	{
		return invertY;
	}
}


#endif