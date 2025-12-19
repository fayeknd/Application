#include "Gamepad.h"
#include "KTYKeyboard.h"
#include "KTYMouse.h"
#include "InputEvents.h"

Input::CursorState Input::Mouse::_cursorState = Input::CursorState::Visible;

POINT Input::Mouse::cPoint;
POINT Input::Mouse::newPoint;

RAWINPUTDEVICE Input::Mouse::mouse_rid;

float Input::Mouse::sensitivity = 0.1f;
bool Input::Mouse::_deltaFlag = false;
bool Input::Mouse::_statechanged = false;

Vector2 Input::Mouse::cPointDelta;
Vector2 Input::Mouse::cPointDeltaLF;
Vector2 Input::Mouse::screenCoordinates;

bool Input::Mouse::enabled = true;
bool Input::Keyboard::enabled = true;
// TODO : Separate this from glfw.

std::vector<Input::InputEvent> Input::InputEvents::events;
DebugLogger Input::InputEvents::ieLogger;


//old stuff
/*
float clamp(float num, float min, float max)
{
	if (num > max)
		num = max;
	if (num < min)
		num = min;
	return num;
}
float Gamepad::length(Vector2 v)
{
	return sqrt((v.x * v.x) + (v.y * v.y));
}
float Gamepad::angle(Vector2 v1, Vector2 v2)
{
	float dot = v1.x * v2.x + v1.y * v2.y;
	float det = v1.x * v2.y - v1.y * v2.x;
	return atan2(det, dot);
}
Vector2 Gamepad::rotate(Vector2 v, float angleInRads) 
{
		float s = sin(angleInRads);
		float c = cos(angleInRads);
		Vector2 r;
		r.x = v.x * c - v.y * s;
		r.y = v.x * s + v.y * c;
		return r;
}
Vector2 Gamepad::CalculateDeadZone(Vector2 v) {
	float len = length(v);
	if (len < deadZone)
		return Vector2(0);
	return v;
}
Vector2 Gamepad::LEFT_STICK(double ft)
{
	Vector2 ret = Vector2(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_X], padState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
	ret = CalculateDeadZone(ret);
	if (ret.x != ret.x)
		ret.x = 0;

	if (ret.y != ret.y)
		ret.y = 0;
	ret = Vector2(clamp(ret.x, -1, 1), clamp(ret.y, -1, 1));

	if (length(ret) > 1)
		ret = rotate(Vector2(1, 0), -angle(ret, Vector2(1, 0)));

	LS_X_ADDITIVE += ret.x * Sensitivity * ft;
	LS_Y_ADDITIVE += ret.y * Sensitivity * ft;
	return ret;
}
Vector2 Gamepad::RIGHT_STICK(double ft)
{
	Vector2 ret = Vector2(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
	ret = CalculateDeadZone(ret);
	if (ret.x != ret.x)
		ret.x = 0;

	if (ret.y != ret.y)
		ret.y = 0;
	ret = Vector2(clamp(ret.x, -1, 1), clamp(ret.y, -1, 1));

	if (length(ret) > 1) 
		ret = rotate(Vector2(1, 0), -angle(ret, Vector2(1, 0)));

	RS_X_ADDITIVE += ret.x * Sensitivity * ft;
	RS_Y_ADDITIVE += ret.y * Sensitivity * ft;
	return ret;
}*/