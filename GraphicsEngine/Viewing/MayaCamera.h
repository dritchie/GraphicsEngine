#pragma once

#ifndef GRAPHICSENGINE__MAYA_CAMERA_H
#define GRAPHICSENGINE__MAYA_CAMERA_H


#include "Viewing/Camera.h"


namespace GraphicsEngine
{
	/**
	Maya-style camera (Alt + mouse buttons for orbit, dolly and, zoom, plus
	scroll wheel for zoom)
	**/
	class MayaCamera : public InteractiveCamera
	{
	public:

		MayaCamera();
		MayaCamera(const Eigen::Vector3f& eye, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up);

		// Accessors
		const float& OrbitSpeed() const;
		float& OrbitSpeed();
		const float& DollySpeed() const;
		float& DollySpeed();
		const float& ZoomSpeed() const;
		float& ZoomSpeed();
		const float& ScrollZoomSpeed() const;
		float& ScrollZoomSpeed();

		// Mouse events
		bool MouseDown(int button, int x, int y, const ModifierKeys& mods);
		bool MouseDrag(int button, int x, int y, const ModifierKeys& mods);
		bool MouseWheel(int delta, const ModifierKeys& mods);

	private:
		float orbitSpeed, dollySpeed, zoomSpeed, scrollZoomSpeed;
		int prevx, prevy;
	};


	__forceinline const float& MayaCamera::OrbitSpeed() const
	{
		return orbitSpeed;
	}

	__forceinline float& MayaCamera::OrbitSpeed()
	{
		return orbitSpeed;
	}
	__forceinline const float& MayaCamera::DollySpeed() const
	{
		return dollySpeed;
	}

	__forceinline float& MayaCamera::DollySpeed()
	{
		return dollySpeed;
	}
	__forceinline const float& MayaCamera::ZoomSpeed() const
	{
		return zoomSpeed;
	}

	__forceinline float& MayaCamera::ZoomSpeed()
	{
		return zoomSpeed;
	}
	__forceinline const float& MayaCamera::ScrollZoomSpeed() const
	{
		return scrollZoomSpeed;
	}

	__forceinline float& MayaCamera::ScrollZoomSpeed()
	{
		return scrollZoomSpeed;
	}
}

#endif