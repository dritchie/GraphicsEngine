#include "Viewing/TinkercadCamera.h"
#include "Math/Math.h"


using namespace Eigen;


#define ORBITSPEED_DEFAULT 0.01f
#define DOLLYSPEED_DEFAULT 0.015f
#define SCROLLZOOMSPEED_DEFAULT 0.8f


namespace GraphicsEngine
{
	TinkercadCamera::TinkercadCamera()
		: InteractiveCamera(),
		  orbitSpeed(ORBITSPEED_DEFAULT), dollySpeed(DOLLYSPEED_DEFAULT),
		  scrollZoomSpeed(SCROLLZOOMSPEED_DEFAULT)
	{
	}

	TinkercadCamera::TinkercadCamera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up, const Vector3f& worldUp)
		: InteractiveCamera(eye,lookAt,up,worldUp),
		  orbitSpeed(ORBITSPEED_DEFAULT), dollySpeed(DOLLYSPEED_DEFAULT),
		  scrollZoomSpeed(SCROLLZOOMSPEED_DEFAULT)
	{
	}

	bool TinkercadCamera::MouseDown(int button, int x, int y, const ModifierKeys& mods)
	{
		prevx = x;
		prevy = y;
		return false;
	}

	bool TinkercadCamera::MouseDrag(int button, int x, int y, const ModifierKeys& mods)
	{
		int dx = x - prevx;
		int dy = y - prevy;

		if (button != MOUSE_LEFT_BUTTON)
		{
			if (button == MOUSE_RIGHT_BUTTON)
			{
				OrbitLeft(-dx * orbitSpeed);
				OrbitUp(dy * orbitSpeed);
			}
			else if (button == MOUSE_MIDDLE_BUTTON)
			{
				DollyLeft(dx * dollySpeed);
				DollyUp(dy * dollySpeed);
			}

			prevx = x;
			prevy = y;
			return true;
		}
		return false;
	}

	bool TinkercadCamera::MouseWheel(int delta, const ModifierKeys& mods)
	{
		Zoom(-delta * scrollZoomSpeed);
		return true;
	}
}