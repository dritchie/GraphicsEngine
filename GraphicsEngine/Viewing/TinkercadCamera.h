#pragma once

#ifndef GRAPHICSENGINE__TINKERCAD_CAMERA_H
#define GRAPHICSENGINE__TINKERCAD_CAMERA_H


#include "Viewing/Camera.h"


namespace GraphicsEngine
{
	/**
	Camera like the one used in Tinkercad (or WebSceneStudio)
	**/
	class TinkercadCamera : public InteractiveCamera
	{
	public:

		TinkercadCamera();
		TinkercadCamera(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up, const Eigen::Vector3f& worldUp = Eigen::Vector3f::UnitY());

		// Accessors
		const float& OrbitSpeed() const;
		float& OrbitSpeed();
		const float& DollySpeed() const;
		float& DollySpeed();
		const float& ScrollZoomSpeed() const;
		float& ScrollZoomSpeed();

		// Mouse events
		bool MouseDown(int button, int x, int y, const ModifierKeys& mods);
		bool MouseDrag(int button, int x, int y, const ModifierKeys& mods);
		bool MouseWheel(int delta, const ModifierKeys& mods);

	private:
		float orbitSpeed, dollySpeed, scrollZoomSpeed;
		int prevx, prevy;
	};


	__forceinline const float& TinkercadCamera::OrbitSpeed() const
	{
		return orbitSpeed;
	}

	__forceinline float& TinkercadCamera::OrbitSpeed()
	{
		return orbitSpeed;
	}
	__forceinline const float& TinkercadCamera::DollySpeed() const
	{
		return dollySpeed;
	}

	__forceinline float& TinkercadCamera::DollySpeed()
	{
		return dollySpeed;
	}
	__forceinline const float& TinkercadCamera::ScrollZoomSpeed() const
	{
		return scrollZoomSpeed;
	}

	__forceinline float& TinkercadCamera::ScrollZoomSpeed()
	{
		return scrollZoomSpeed;
	}
}

#endif