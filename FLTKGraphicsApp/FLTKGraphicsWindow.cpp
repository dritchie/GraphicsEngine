#include "FLTKGraphicsWindow.h"
#include "FL/Fl.H"


using namespace GraphicsEngine;


FLTKGraphicsWindow::FLTKGraphicsWindow(GraphicsApp* gApp, int x, int y, int w, int h, const char *l)
	: Fl_Gl_Window(x,y,w,h,l), app(gApp), heldButton(-1), heldKey(-1)
{
}

void FLTKGraphicsWindow::draw()
{
	if (!valid())
	{	
		app->Resize(w(), h());
	}

	app->Render();
}

int FLTKGraphicsWindow::handle(int eventType)
{
	// Get the current event state, convert to
	// GraphicsEngine format
	int eventState = Fl::event_state();
	ModifierKeys mods;
	mods.alt = (eventState & FL_ALT) != 0;
	mods.ctrl = (eventState & FL_CTRL) != 0;
	mods.shift = (eventState & FL_SHIFT) != 0;
	
	// Check if keys/mousebuttons were pressed,
	// convert to GraphicsEngine format
	int mouseButton = ConvertMouseButtonCode(Fl::event_button());
	int mouseWheel = Fl::event_dy();
	int key = ConvertKeyCode(Fl::event_key());
	int x = Fl::event_x();
	int y = Fl::event_y();


	// Forward all relevant events to the app
	switch(eventType)
	{
	case FL_PUSH:
		app->MouseDown(mouseButton, x, y, mods);
		heldButton = mouseButton;
		return 1;
	case FL_RELEASE:
		app->MouseUp(mouseButton, x, y, mods);
		heldButton = -1;
		return 1;
	case FL_MOVE:
		app->MouseMove(x, y, mods);
		return 1;
	case FL_DRAG:
		app->MouseDrag(heldButton, x, y, mods);
		return 1;
	case FL_MOUSEWHEEL:
		app->MouseWheel(mouseWheel, mods);
		return 1;
	case FL_KEYDOWN:
		if (key != -1)
		{
			// FLTK generates repeated KEYDOWN events for held keys.
			// Do not propagate these.
			if (key != heldKey)
			{
				app->KeyDown(key, mods);
				heldKey = key;
			}
			return 1;
		}
		else return 0;
	case FL_KEYUP:
		if (key != -1)
		{
			app->KeyUp(key, mods);
			if (key == heldKey)
				heldKey = -1;
			return 1;
		}
		else return 0;
	default:
		return Fl_Gl_Window::handle(eventType);
	}
}

int FLTKGraphicsWindow::ConvertMouseButtonCode(int mouseButton)
{
	switch(mouseButton)
	{
	case FL_LEFT_MOUSE:
		return MOUSE_LEFT_BUTTON;
	case FL_MIDDLE_MOUSE:
		return MOUSE_MIDDLE_BUTTON;
	case FL_RIGHT_MOUSE:
		return MOUSE_RIGHT_BUTTON;
	default:
		return -1;
	}
}

int FLTKGraphicsWindow::ConvertKeyCode(int key)
{
	// Various special keys
	if (key == FL_Escape)
		return KEY_ESCAPE;
	if (key == FL_BackSpace)
		return KEY_BACKSPACE;
	if (key == FL_Tab)
		return KEY_TAB;
	if (key == FL_Enter || key == FL_KP_Enter)
		return KEY_ENTER;
	if (key == FL_Pause)
		return KEY_PAUSE;
	if (key == FL_Insert)
		return KEY_INSERT;
	if (key == FL_Home)
		return KEY_HOME;
	if (key == FL_Page_Up)
		return KEY_PGUP;
	if (key == FL_Delete)
		return KEY_DELETE;
	if (key == FL_End)
		return KEY_END;
	if (key == FL_Page_Down)
		return KEY_PGDN;
	
	// Spacebar
	if (key == 32)
		return KEY_SPACE;

	// Arrow keys
	if (key == FL_Left)
		return KEY_LEFT;
	if (key == FL_Right)
		return KEY_RIGHT;
	if (key == FL_Up)
		return KEY_UP;
	if (key == FL_Down)
		return KEY_DOWN;

	// Function keys
	if (key > FL_F && key <= (FL_F + 12))
		return (KEY_F1 - 1) + (key - FL_F);

	// Keypad keys producing ASCII values
	if (key > FL_KP && key <= FL_KP_Last)
		return key - FL_KP;

	// Filter out modifier keys
	if (key == FL_Shift_L || key == FL_Shift_R ||
		key == FL_Alt_L || key == FL_Alt_R ||
		key == FL_Control_L || key == FL_Control_R ||
		key == FL_Num_Lock || key == FL_Scroll_Lock || key == FL_Caps_Lock)
		return -1;

	// Normal keys producing ASCII values
	return key;
}

void FLTKGraphicsWindow::Redraw()
{
	redraw();
}

void FLTKGraphicsWindow::MakeCurrent()
{
	make_current();
}