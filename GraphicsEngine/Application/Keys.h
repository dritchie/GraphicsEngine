#pragma once

#ifndef GRAPHICSENGINE__KEYS_H
#define GRAPHICSENGINE__KEYS_H


namespace GraphicsEngine
{
	/**
	Struct that stores application modifer key status
	**/
	struct ModifierKeys
	{
		bool shift;
		bool ctrl;
		bool alt;
		ModifierKeys() : shift(false), ctrl(false), alt(false) {}
	};
}


/**
Window-system-independent keycode and mouse button definitions
(Adapted from code by Matthew Fisher)

For keys not mentioned here, just use ASCII codes
(i.e. 'a' for  A, '/' for slash)
**/

#define MOUSE_LEFT_BUTTON	  1
#define MOUSE_MIDDLE_BUTTON	  2
#define MOUSE_RIGHT_BUTTON	  3

#define KEY_SPACE             32

#define KEY_ESCAPE            256
#define KEY_BACKSPACE         257
#define KEY_TAB               258
#define KEY_ENTER             259
#define KEY_PAUSE             260
#define KEY_INSERT            261
#define KEY_HOME              262
#define KEY_PGUP              263
#define KEY_DELETE            264
#define KEY_END               265
#define KEY_PGDN              266

#define KEY_LEFT              267
#define KEY_UP                268
#define KEY_RIGHT             269
#define KEY_DOWN              270

#define KEY_F1                271
#define KEY_F2                272
#define KEY_F3                273
#define KEY_F4                274
#define KEY_F5                275
#define KEY_F6                276
#define KEY_F7                277
#define KEY_F8                278
#define KEY_F9                279
#define KEY_F10               280
#define KEY_F11               281
#define KEY_F12               282


#endif