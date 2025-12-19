#pragma once
#include "Entity.h";
#include "KTYKeyboard.h"
#include "KTYMouse.h"
#include "Camera.h"
#include "ScreenshotPNG.h"
#include "InputEvents.h"

using namespace Input;

class CameraController : public EntityComponent {
public:
	static inline Vector2 pos;
	static inline float mult = 3;


	static void MoveForward(float v) {
		Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.forward() * mult;
	}
	static void MoveBackward(float v) {
		Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.back() * mult;
	}

	static void MoveLeft(float v) {
		Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.left() * mult;
	}
	static void MoveRight(float v) {
		Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.right() * mult;
	}

	static void MoveUp(float v) {
		Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.up() * mult;
	}
	static void MoveDown(float v) {
		Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.down() * mult;
	}

	static void MouseLeftClick(float v) {
		pos = Mouse::GetCursorPosScreen();
		Mouse::SetCursorState(CursorState::Disabled);
	}
	static void MouseLeftPress(float v) {
		Camera::mainCamera->Look(Mouse::CursorDelta());
	}
	static void MouseLeftRelease(float v) {
		Mouse::SetCursorPosScreen(pos);
		Mouse::SetCursorState(CursorState::Visible);
	}

	static void TakeScreenshot(float val) {
		Screenshot::TakeScreenshot();
	}

	void Start() override {
		
		InputEvents::RegisterEvent("Forward", std::vector<int>{key_W, key_up})->RegisterFunc(pressed, MoveForward);
		InputEvents::RegisterEvent("Backward", std::vector<int>{key_S, key_down})->RegisterFunc(pressed, MoveBackward);

		InputEvents::RegisterEvent("Left", std::vector<int>{key_A, key_left})->RegisterFunc(pressed, MoveLeft);
		InputEvents::RegisterEvent("Right", std::vector<int>{key_D, key_right})->RegisterFunc(pressed, MoveRight);
		
		InputEvents::RegisterEvent("Up", std::vector<int>{key_E, key_space})->RegisterFunc(pressed, MoveUp);
		InputEvents::RegisterEvent("Down", std::vector<int>{key_Q, key_left_shift})->RegisterFunc(pressed, MoveDown);

		InputEvents::RegisterEvent("TakeScreenshot", key_f12)->RegisterFunc(pressed_this_frame, TakeScreenshot);

		InputEvent* rm = InputEvents::RegisterEvent("RightMouse", std::vector<int>{mouse_right, key_G});
		rm->RegisterFunc(pressed_this_frame, MouseLeftClick);
		rm->RegisterFunc(depressed_this_frame, MouseLeftRelease);
		rm->RegisterFunc(pressed, MouseLeftPress);
	}

};