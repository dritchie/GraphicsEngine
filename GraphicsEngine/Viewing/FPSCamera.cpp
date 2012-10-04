#include "Viewing/FPSCamera.h"


using namespace Eigen;


#define MOVESPEED_DEFAULT 10.0f
#define LOOKSPEED_DEFAULT 0.01f


namespace GraphicsEngine
{
	FPSCamera::FPSCamera()
		: InteractiveCamera(), movingForward(false), movingBackward(false),
		  movingLeft(false), movingRight(false),
		  moveSpeed(MOVESPEED_DEFAULT), lookSpeed(LOOKSPEED_DEFAULT),
		  invertY(false)
	{
	}

	FPSCamera::FPSCamera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up, const Vector3f& worldUp)
		: InteractiveCamera(eye,lookAt,up, worldUp), movingForward(false), movingBackward(false),
		  movingLeft(false), movingRight(false),
		  moveSpeed(MOVESPEED_DEFAULT), lookSpeed(LOOKSPEED_DEFAULT),
		  invertY(false)
	{
	}

	bool FPSCamera::KeyDown(int key, const ModifierKeys& mods)
	{
		switch(key)
		{
		case 'w':
			movingForward = true;
			break;
		case 's':
			movingBackward = true;
			break;
		case 'a':
			movingLeft = true;
			break;
		case 'd':
			movingRight = true;
			break;
		default:
			break;
		}

		return false;
	}

	bool FPSCamera::KeyUp(int key, const ModifierKeys& mods)
	{
		switch(key)
		{
		case 'w':
			movingForward = false;
			break;
		case 's':
			movingBackward = false;
			break;
		case 'a':
			movingLeft = false;
			break;
		case 'd':
			movingRight = false;
			break;
		default:
			break;
		}

		return false;
	}

	bool FPSCamera::MouseDown(int button, int x, int y, const ModifierKeys& mods)
	{
		if (button == MOUSE_RIGHT_BUTTON)
		{
			prevx = x;
			prevy = y;
		}
		return false;
	}

	bool FPSCamera::MouseDrag(int button, int x, int y, const ModifierKeys& mods)
	{
		if (button == MOUSE_RIGHT_BUTTON)
		{
			int dx = x - prevx;
			int dy = y - prevy;
			PanLeft(-dx * lookSpeed);
			PanUp(dy * lookSpeed);
			prevx = x;
			prevy = y;
			return true;
		}
		return false;
	}

	bool FPSCamera::Timestep(float dt)
	{
		if (movingForward)
		{
			DollyForward(dt * moveSpeed);
			return true;
		}
		else if (movingBackward)
		{
			DollyForward(-dt * moveSpeed);
			return true;
		}
		else if (movingLeft)
		{
			DollyLeft(dt * moveSpeed);
			return true;
		}
		else if (movingRight)
		{
			DollyLeft(-dt * moveSpeed);
			return true;
		}
		else return false;
	}
}