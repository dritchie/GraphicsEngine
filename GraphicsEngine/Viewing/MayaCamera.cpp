#include "Viewing/MayaCamera.h"
#include "Math/Math.h"


using namespace Eigen;


#define ORBITSPEED_DEFAULT 0.01f
#define DOLLYSPEED_DEFAULT 0.015f
#define ZOOMSPEED_DEFAULT 0.02f
#define SCROLLZOOMSPEED_DEFAULT 0.8f


namespace GraphicsEngine
{
	MayaCamera::MayaCamera()
		: InteractiveCamera(),
		  orbitSpeed(ORBITSPEED_DEFAULT), dollySpeed(DOLLYSPEED_DEFAULT),
		  zoomSpeed(ZOOMSPEED_DEFAULT), scrollZoomSpeed(SCROLLZOOMSPEED_DEFAULT)
	{
	}

	MayaCamera::MayaCamera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up)
		: InteractiveCamera(eye,lookAt,up),
		  orbitSpeed(ORBITSPEED_DEFAULT), dollySpeed(DOLLYSPEED_DEFAULT),
		  zoomSpeed(ZOOMSPEED_DEFAULT), scrollZoomSpeed(SCROLLZOOMSPEED_DEFAULT)
	{
	}

	bool MayaCamera::MouseDown(int button, int x, int y, const ModifierKeys& mods)
	{
		if (mods.alt)
		{
			prevx = x;
			prevy = y;
		}
		return false;
	}

	bool MayaCamera::MouseDrag(int button, int x, int y, const ModifierKeys& mods)
	{
		if (mods.alt)
		{
			int dx = x - prevx;
			int dy = y - prevy;

			if (button == MOUSE_LEFT_BUTTON)
			{
				OrbitLeft(-dx * orbitSpeed);
				OrbitUp(dy * orbitSpeed);
			}
			else if (button == MOUSE_MIDDLE_BUTTON)
			{
				DollyLeft(dx * dollySpeed);
				DollyUp(dy * dollySpeed);
			}
			else if (button == MOUSE_RIGHT_BUTTON)
			{
				int val = (abs(dx) > abs(dy) ? dx : dy);
				Zoom(val * zoomSpeed);
			}

			prevx = x;
			prevy = y;
			return true;
		}
		return false;
	}

	bool MayaCamera::MouseWheel(int delta, const ModifierKeys& mods)
	{
		Zoom(-delta * scrollZoomSpeed);
		return true;
	}
}