#include "input_handler.h"

/*----------------------------------------------------------------------------\
 *						Initialisation and Clean up                   		  |
 *----------------------------------------------------------------------------*/

/*
 Constructor
 */
InputHandler::InputHandler()
{
	keyboardMap[KEY_A] = KEY_NOT_PRESSED;
	keyboardMap[KEY_B] = KEY_NOT_PRESSED;
	keyboardMap[KEY_C] = KEY_NOT_PRESSED;
	keyboardMap[KEY_D] = KEY_NOT_PRESSED;
	keyboardMap[KEY_E] = KEY_NOT_PRESSED;
	keyboardMap[KEY_F] = KEY_NOT_PRESSED;
	keyboardMap[KEY_G] = KEY_NOT_PRESSED;
	keyboardMap[KEY_H] = KEY_NOT_PRESSED;
	keyboardMap[KEY_I] = KEY_NOT_PRESSED;
	keyboardMap[KEY_J] = KEY_NOT_PRESSED;
	keyboardMap[KEY_K] = KEY_NOT_PRESSED;
	keyboardMap[KEY_L] = KEY_NOT_PRESSED;
	keyboardMap[KEY_M] = KEY_NOT_PRESSED;
	keyboardMap[KEY_N] = KEY_NOT_PRESSED;
	keyboardMap[KEY_O] = KEY_NOT_PRESSED;
	keyboardMap[KEY_P] = KEY_NOT_PRESSED;
	keyboardMap[KEY_Q] = KEY_NOT_PRESSED;
	keyboardMap[KEY_R] = KEY_NOT_PRESSED;
	keyboardMap[KEY_S] = KEY_NOT_PRESSED;
	keyboardMap[KEY_T] = KEY_NOT_PRESSED;
	keyboardMap[KEY_U] = KEY_NOT_PRESSED;
	keyboardMap[KEY_V] = KEY_NOT_PRESSED;
	keyboardMap[KEY_W] = KEY_NOT_PRESSED;
	keyboardMap[KEY_X] = KEY_NOT_PRESSED;
	keyboardMap[KEY_Y] = KEY_NOT_PRESSED;
	keyboardMap[KEY_Z] = KEY_NOT_PRESSED;

	keyboardMap[KEY_SPACE_BAR] = KEY_NOT_PRESSED;

	keyboardMap[KEY_ONE] = KEY_NOT_PRESSED;
	keyboardMap[KEY_TWO] = KEY_NOT_PRESSED;
	keyboardMap[KEY_THREE] = KEY_NOT_PRESSED;
	keyboardMap[KEY_FOUR] = KEY_NOT_PRESSED;
	keyboardMap[KEY_FIVE] = KEY_NOT_PRESSED;
	keyboardMap[KEY_SIX] = KEY_NOT_PRESSED;
	keyboardMap[KEY_SEVEN] = KEY_NOT_PRESSED;
	keyboardMap[KEY_EIGHT] = KEY_NOT_PRESSED;
	keyboardMap[KEY_NINE] = KEY_NOT_PRESSED;
	keyboardMap[KEY_ZERO] = KEY_NOT_PRESSED;

	keyboardMap[KEY_ARROW_UP] = KEY_NOT_PRESSED;
	keyboardMap[KEY_ARROW_DOWN] = KEY_NOT_PRESSED;
	keyboardMap[KEY_ARROW_LEFT] = KEY_NOT_PRESSED;
	keyboardMap[KEY_ARROW_RIGHT] = KEY_NOT_PRESSED;

	keyboardMap[KEY_PAGE_UP] = KEY_NOT_PRESSED;
	keyboardMap[KEY_PAGE_DOWN] = KEY_NOT_PRESSED;

	keyboardMap[KEY_ADD] = KEY_NOT_PRESSED;
	keyboardMap[KEY_SUBTRACT] = KEY_NOT_PRESSED;

	mouseButtonMap[MOUSE_LBUTTON] = KEY_NOT_PRESSED;
	mouseButtonMap[MOUSE_MBUTTON] = KEY_NOT_PRESSED;
	mouseButtonMap[MOUSE_RBUTTON] = KEY_NOT_PRESSED;

	mouseButtonMap[KEY_CONTROL] = KEY_NOT_PRESSED;
}

/*
 Destructor
 */
InputHandler::~InputHandler()
{
}

/*----------------------------------------------------------------------------\
 *								Update keystrokes                   		  |
 *----------------------------------------------------------------------------*/

/*
 Updates the input handler for this frame
 */
void InputHandler::updateInputHandler()
{
	KeyboardMap::iterator keyboardIter;   
	
	// Rotate the button states now this frame is over - i.e. newly pressed becomes held
	for( keyboardIter = keyboardMap.begin(); keyboardIter != keyboardMap.end(); keyboardIter++ ) 
	{
		keyboardIter->second = updateKeyState(keyboardIter->second);
	}

	MouseButtonMap::iterator mouseIter;   
	
	// Rotate the button states now this frame is over - i.e. newly pressed becomes held
	for( mouseIter = mouseButtonMap.begin(); mouseIter != mouseButtonMap.end(); mouseIter++ ) 
	{
		mouseIter->second = updateKeyState(mouseIter->second);
	}

	// reset the mouse to ensure that deltas are carried over to the next frame if there are no mouse events.
	resetMouse();
}

/*----------------------------------------------------------------------------\
 *								Handle input events                   		  |
 *----------------------------------------------------------------------------*/

/*
 Sends a keyboard event to the input handler

 Parameter list
 	wParam:				the win api wParam ascociated with this keyboard event
	eventName:			the win api event name for this event
 */
void InputHandler::sendKeyboardEvent(WPARAM wParam, UINT eventName)
{
	switch(wParam)
	{
		case 0x41:
			keyboardMap[KEY_A] = resolveButtonState(keyboardMap[KEY_A], eventName);
			break;

		case 0x42:
			keyboardMap[KEY_B] = resolveButtonState(keyboardMap[KEY_B], eventName);
			break;

		case 0x43:
			keyboardMap[KEY_C] = resolveButtonState(keyboardMap[KEY_C], eventName);
			break;

		case 0x44:
			keyboardMap[KEY_D] = resolveButtonState(keyboardMap[KEY_D], eventName);
			break;

		case 0x45:
			keyboardMap[KEY_E] = resolveButtonState(keyboardMap[KEY_E], eventName);
			break;

		case 0x46:
			keyboardMap[KEY_F] = resolveButtonState(keyboardMap[KEY_F], eventName);
			break;

		case 0x47:
			keyboardMap[KEY_G] = resolveButtonState(keyboardMap[KEY_G], eventName);
			break;

		case 0x48:
			keyboardMap[KEY_H] = resolveButtonState(keyboardMap[KEY_H], eventName);
			break;

		case 0x49:
			keyboardMap[KEY_I] = resolveButtonState(keyboardMap[KEY_I], eventName);
			break;

		case 0x4A:
			keyboardMap[KEY_J] = resolveButtonState(keyboardMap[KEY_J], eventName);
			break;

		case 0x4B:
			keyboardMap[KEY_K] = resolveButtonState(keyboardMap[KEY_K], eventName);
			break;

		case 0x4C:
			keyboardMap[KEY_L] = resolveButtonState(keyboardMap[KEY_L], eventName);
			break;

		case 0x4D:
			keyboardMap[KEY_M] = resolveButtonState(keyboardMap[KEY_M], eventName);
			break;

		case 0x4E:
			keyboardMap[KEY_N] = resolveButtonState(keyboardMap[KEY_N], eventName);
			break;

		case 0x4F:
			keyboardMap[KEY_O] = resolveButtonState(keyboardMap[KEY_O], eventName);
			break;

		case 0x50:
			keyboardMap[KEY_P] = resolveButtonState(keyboardMap[KEY_P], eventName);
			break;

		case 0x51:
			keyboardMap[KEY_Q] = resolveButtonState(keyboardMap[KEY_Q], eventName);
			break;

		case 0x52:
			keyboardMap[KEY_R] = resolveButtonState(keyboardMap[KEY_R], eventName);
			break;

		case 0x53:
			keyboardMap[KEY_S] = resolveButtonState(keyboardMap[KEY_S], eventName);
			break;

		case 0x54:
			keyboardMap[KEY_T] = resolveButtonState(keyboardMap[KEY_T], eventName);
			break;

		case 0x55:
			keyboardMap[KEY_U] = resolveButtonState(keyboardMap[KEY_U], eventName);
			break;

		case 0x56:
			keyboardMap[KEY_V] = resolveButtonState(keyboardMap[KEY_V], eventName);
			break;

		case 0x57:
			keyboardMap[KEY_W] = resolveButtonState(keyboardMap[KEY_W], eventName);
			break;

		case 0x58:
			keyboardMap[KEY_X] = resolveButtonState(keyboardMap[KEY_X], eventName);
			break;

		case 0x59:
			keyboardMap[KEY_Y] = resolveButtonState(keyboardMap[KEY_Y], eventName);
			break;

		case 0x5A:
			keyboardMap[KEY_Z] = resolveButtonState(keyboardMap[KEY_Z], eventName);
			break;

		case 0x30:
			keyboardMap[KEY_ZERO] = resolveButtonState(keyboardMap[KEY_ZERO], eventName);
			break;

		case 0x31:
			keyboardMap[KEY_ONE] = resolveButtonState(keyboardMap[KEY_ONE], eventName);
			break;

		case 0x32:
			keyboardMap[KEY_TWO] = resolveButtonState(keyboardMap[KEY_TWO], eventName);
			break;

		case 0x33:
			keyboardMap[KEY_THREE] = resolveButtonState(keyboardMap[KEY_THREE], eventName);
			break;

		case 0x34:
			keyboardMap[KEY_FOUR] = resolveButtonState(keyboardMap[KEY_FOUR], eventName);
			break;

		case 0x35:
			keyboardMap[KEY_FIVE] = resolveButtonState(keyboardMap[KEY_FIVE], eventName);
			break;

		case 0x36:
			keyboardMap[KEY_SIX] = resolveButtonState(keyboardMap[KEY_SIX], eventName);
			break;

		case 0x37:
			keyboardMap[KEY_SEVEN] = resolveButtonState(keyboardMap[KEY_SEVEN], eventName);
			break;

		case 0x38:
			keyboardMap[KEY_EIGHT] = resolveButtonState(keyboardMap[KEY_EIGHT], eventName);
			break;

		case 0x39:
			keyboardMap[KEY_NINE] = resolveButtonState(keyboardMap[KEY_NINE], eventName);
			break;

		case VK_UP:
			keyboardMap[KEY_ARROW_UP] = resolveButtonState(keyboardMap[KEY_ARROW_UP], eventName);
			break;

		case VK_DOWN:
			keyboardMap[KEY_ARROW_DOWN] = resolveButtonState(keyboardMap[KEY_ARROW_DOWN], eventName);
			break;

		case VK_LEFT:
			keyboardMap[KEY_ARROW_LEFT] = resolveButtonState(keyboardMap[KEY_ARROW_LEFT], eventName);
			break;

		case VK_RIGHT:
			keyboardMap[KEY_ARROW_RIGHT] = resolveButtonState(keyboardMap[KEY_ARROW_RIGHT], eventName);
			break;

		case VK_PRIOR:
			keyboardMap[KEY_PAGE_UP] = resolveButtonState(keyboardMap[KEY_PAGE_UP], eventName);
			break;

		case VK_NEXT:
			keyboardMap[KEY_PAGE_DOWN] = resolveButtonState(keyboardMap[KEY_PAGE_DOWN], eventName);
			break;

		case VK_OEM_PLUS:
			keyboardMap[KEY_ADD] = resolveButtonState(keyboardMap[KEY_ADD], eventName);
			break;

		case VK_OEM_MINUS:
			keyboardMap[KEY_SUBTRACT] = resolveButtonState(keyboardMap[KEY_SUBTRACT], eventName);
			break;

		case VK_SPACE:
			keyboardMap[KEY_SPACE_BAR] = resolveButtonState(keyboardMap[KEY_SPACE_BAR], eventName);
			break;

		case VK_CONTROL:
			keyboardMap[KEY_CONTROL] = resolveButtonState(keyboardMap[KEY_CONTROL], eventName);
			break;
	}
}

/*
 Resets the mouse delta's
 */
void InputHandler::resetMouse()
{
	mousePosDelta.x = 0;
	mousePosDelta.y = 0;
	mousePosDelta.z = 0;

	mouseWheelDelta = 0;
}

/*
 Sends a mouse event to the input handler

 Parameter list
 	wParam:				the win api wParam ascociated with this keyboard event
	lParam:				the win api lParam ascociated with this keyboard event
	eventName:			the win api event name for this event
 */
void InputHandler::sendMouseEvent(WPARAM wParam, LPARAM lParam, UINT eventName)
{
	bool allowDeltaUpdate = false;
	resetMouse();
	
	switch(eventName)
	{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			mouseButtonMap[MOUSE_LBUTTON] = resolveButtonState(mouseButtonMap[MOUSE_LBUTTON], eventName);
			allowDeltaUpdate = true;
			break;

		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			mouseButtonMap[MOUSE_MBUTTON] = resolveButtonState(mouseButtonMap[MOUSE_MBUTTON], eventName);
			allowDeltaUpdate = true;
			break;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			mouseButtonMap[MOUSE_RBUTTON] = resolveButtonState(mouseButtonMap[MOUSE_RBUTTON], eventName);
			allowDeltaUpdate = true;
			break;
	}

	// only update the mouse delta when there is a button pressed down or there was a mouse button event.
	if(allowDeltaUpdate || wParam != 0)
	{
		setNewMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else
	{
		// if there are no mouse buttons down - keep track of the current mouse position so that
		// there isn't a large delta created between where the user has clicked on the screen.
		mousePos.x = static_cast<float>( GET_X_LPARAM(lParam) );
		mousePos.y = static_cast<float>( GET_Y_LPARAM(lParam) );
	}
}

/*
 Sends a mouse wheel event to the input handler

 Parameter list
 	wParam:				the win api wParam ascociated with this keyboard event
 */
void InputHandler::sendMouseWheelEvent(WPARAM wParam)
{
	mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
}

/*
 Sets the current mouse position and updates the position delta's

 Parameter list
 	x:				the x screen coordinate
	y:				the y screen coordinate
 */
void InputHandler::setNewMousePos( int x, int y )
{
	//mousePos
	mousePosDelta.x = x - mousePos.x;
	mousePosDelta.y = y - mousePos.y;

	mousePos.x = static_cast< float >( x );
	mousePos.y = static_cast< float >( y );
}

/*
 Resolves the current button state of a button based on its old and new states

 Parameter list
 	current:			the current input state of the button
	newState:			the new input state of the button
 */
ButtonState InputHandler::resolveButtonState(ButtonState current, UINT newState)
{
	// if it was a key down event - need to test for mouse mouse button downs aswell so the function can be recycled
	if(newState == WM_KEYDOWN 
		|| newState == WM_LBUTTONDOWN 
		|| newState == WM_MBUTTONDOWN 
		|| newState == WM_RBUTTONDOWN 
		|| newState == WM_XBUTTONDOWN)
	{
		if(current == KEY_NOT_PRESSED || current == KEY_RELEASED)
		{
			return KEY_NEWLY_PRESSED;
		}
		else
		{
			return KEY_HELD;
		}
	}
	else if(newState == WM_KEYUP
		|| newState == WM_LBUTTONUP // if it was a key down event - need to test for mouse
		|| newState == WM_MBUTTONUP // mouse button ups aswell so the function can be recycled
		|| newState == WM_RBUTTONUP 
		|| newState == WM_XBUTTONUP)
	{
		if(current == KEY_HELD || current == KEY_NEWLY_PRESSED)
		{
			return KEY_RELEASED;
		}
		else
		{
			return KEY_NOT_PRESSED;
		}
	}
	else
	{
		return KEY_NOT_PRESSED;
	}
}

/*
 Cycles the button state to it's next state e.g. from newly pressed to held.
 This function is used to update keys on a frame by frame basis to cope with
 not having repeating win api events for held keys.

 Parameter list
 	current:			the current input state of the button
 */
ButtonState InputHandler::updateKeyState(ButtonState current)
{
	if(current == KEY_NEWLY_PRESSED || current == KEY_HELD)
	{
		return KEY_HELD;
	}
	else if(current == KEY_RELEASED)
	{
		return KEY_NOT_PRESSED;
	}
	else
	{
		return KEY_NOT_PRESSED;
	}
}