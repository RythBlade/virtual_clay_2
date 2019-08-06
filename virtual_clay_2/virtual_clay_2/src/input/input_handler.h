#pragma once

#include <string>
#include <map>
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <DirectXMath.h>



enum ButtonState
{
	KEY_NOT_PRESSED = 0,
	KEY_RELEASED = 1,
	KEY_HELD = 2,
	KEY_NEWLY_PRESSED = 3,		
};

/*----------------------------------------------------------------------------\
 *								Input Settings                   			  |
 *----------------------------------------------------------------------------*/
#define MOUSE_X_SCROLL_SPEED			0.005f
#define MOUSE_Y_SCROLL_SPEED			-0.005f
#define MOUSE_WHEEL_SCROLL_SPEED		-10.0f

/*----------------------------------------------------------------------------\
 *								Input Indices                   			  |
 *----------------------------------------------------------------------------*/
 /*								Keyboard									  */

#define KEY_A							"a"
#define KEY_B							"b"
#define KEY_C							"c"
#define KEY_D							"d"
#define KEY_E							"e"
#define KEY_F							"f"
#define KEY_G							"g"
#define KEY_H							"h"
#define KEY_I							"i"
#define KEY_J							"j"
#define KEY_K							"k"
#define KEY_L							"l"
#define KEY_M							"m"
#define KEY_N							"n"
#define KEY_O							"o"
#define KEY_P							"p"
#define KEY_Q							"q"
#define KEY_R							"r"
#define KEY_S							"s"
#define KEY_T							"t"
#define KEY_U							"u"
#define KEY_V							"v"
#define KEY_W							"w"
#define KEY_X							"x"
#define KEY_Y							"y"
#define KEY_Z							"z"

#define KEY_SPACE_BAR					"space"

#define KEY_ONE							"1"
#define KEY_TWO							"2"
#define KEY_THREE						"3"
#define KEY_FOUR						"4"
#define KEY_FIVE						"5"
#define KEY_SIX							"6"
#define KEY_SEVEN						"7"
#define KEY_EIGHT						"8"
#define KEY_NINE						"9"
#define KEY_ZERO						"0"

#define KEY_ARROW_UP					"up"
#define KEY_ARROW_DOWN					"down"
#define KEY_ARROW_LEFT					"left"
#define KEY_ARROW_RIGHT					"right"

#define KEY_PAGE_UP						"pageUp"
#define KEY_PAGE_DOWN					"pageDown"

#define KEY_SUBTRACT					"subtract"
#define KEY_ADD							"add"

#define KEY_CONTROL						"control"

/*								Mouse										  */
#define MOUSE_LBUTTON					"LMB"
#define MOUSE_MBUTTON					"MMB"
#define MOUSE_RBUTTON					"RMB"

typedef std::map<std::string, ButtonState> KeyboardMap, MouseButtonMap;

class InputHandler
{
private:
	KeyboardMap keyboardMap;		// dictionary containing the state of keyboard buttons
	MouseButtonMap mouseButtonMap;	// dictionary containing the state of mouse buttons

	DirectX::XMFLOAT3 mousePos;			// the current mouse position on screen
	DirectX::XMFLOAT3 mousePosDelta;		// the difference in mouse position between this frame and the previous one
	float mouseWheelDelta;			// the delta mouse wheel between this frame and the previous one

public:
	InputHandler();
	~InputHandler();

	void sendKeyboardEvent(WPARAM wParam, UINT eventName);
	void sendMouseEvent(WPARAM wParam, LPARAM lParam, UINT eventName);
	void sendMouseWheelEvent(WPARAM wParam);
	void updateInputHandler();									// called every frame by the manager of the input handler

	// data accessors
	inline KeyboardMap *getKeyboardMap() { return &keyboardMap; }
	inline MouseButtonMap *getMouseButtonMap() { return &mouseButtonMap; }
	inline DirectX::XMFLOAT3*getMousePos() { return &mousePos; }
	inline DirectX::XMFLOAT3*getMousePosDelta() { return &mousePosDelta; }
	inline float getMouseWheelDelta() { return mouseWheelDelta; }

private:
	void resetMouse();
	void setNewMousePos(int x, int y);
	ButtonState resolveButtonState(ButtonState current, UINT newState);
	ButtonState updateKeyState(ButtonState current);
};