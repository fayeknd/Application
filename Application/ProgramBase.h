#ifndef GLFW_WINDOW
#define GLFW_WINDOW

#include "Logger.h"

#include "KTYMath.h"
#include <sysinfoapi.h>
#include <sstream>
#include "DeviceInfo.h"
#include "KTYKeyboard.h"
#include "KTYMouse.h"
#include "KTYTime.h"
#include "Shader.h"
#include "Entity.h"
#include "global.h"
#include "DefaultShaders.h"
#include "Camera.h"
#include "Light.h"
#include "ScreenshotPNG.h"
#include "Framebuffer.h"
#include "InputEvents.h"

// Am aware having a program class inherit from "program" is not needed anymore but oh well

using namespace KTYTime;

class Program {
private:
	double frameRate = 0;
	Time::Timer deltaTimer;
	bool _vsyncState = false;
public:

	GLFWmonitor* monitor;
	GLFWwindow* thisWindow;
	int currentFrame = 0;
	double targetFrameRate = 60;
	double FrameRate() const { return frameRate; }
	bool VsyncState() const { return _vsyncState; }
	DebugLogger GLFWLog;

	template <typename T> 
	void SetVsyncState(T val) {}

	template <> void SetVsyncState<int>(int val) {
		if (val == 0) {
			_vsyncState = false;
			glfwSwapInterval(0);
			return;
		}
		_vsyncState = true;
		glfwSwapInterval(1);
	}
	template <> void SetVsyncState<bool>(bool val) {
		_vsyncState = val;
		if (val) {
			glfwSwapInterval(1);
			return; 
		}
		glfwSwapInterval(0);
	}
	Vector2 GetWindowPos() {
		int x, y;
		glfwGetWindowPos(thisWindow, &x, &y);
		return Vector2(x, y);
	}
	void SetWindowSize(Vector2 size)
	{
		glfwSetWindowSize(thisWindow, size.x, size.y);
		glfwSetWindowPos(thisWindow, (KTYGlobal::MonitorInfo()->width - KTYGlobal::windowSize.x) / 2 /* x padding */, (KTYGlobal::MonitorInfo()->height - KTYGlobal::windowSize.y) / 2 /* y padding */);
	}
	void UpdateWindowSize()
	{
		int width, height;
		glfwGetWindowSize(thisWindow, &width, &height);
		if (width == 0 || height == 0)
			return;
		KTYGlobal::windowSize = Vector2(width, height);
	}

	int InitLogger(bool logEngineInfo = true) {
		if (logEngineInfo)
			KTYGlobal::LogEngineInfo();
		GLFWLog.LoggerName = "GLFW";
		GLFWLog.defaultColour = DBG_CYAN;
		GLFWLog.CLog("GLFW Logger initialised.");
		return 0;
	}

	int Begin(std::string windowName = "GLFW", int winType = KTYGlobal::currentWindowType);
	
	virtual void PreInit() {}
	virtual void Update() {}
	virtual void PostRender() {}
	virtual void Start() {}

	void GeneralComponentInitialisation() {
		
		DefaultShaders::Create();
		KTYGlobal::SetMainWindow(thisWindow);
		SetVsyncState(true);
		Framebuffer::Default = new Framebuffer();

		Start();
		Input::Mouse::SetMouseCallback();
	}

	int MainLoop() {

		while (!glfwWindowShouldClose(thisWindow)) {

			if (!deltaTimer.Counting())
				deltaTimer.StartNew();                             //Starts the deltaTime timer
			if (deltaTimer.Value() >= ((double)1 / Math::clamp((double)targetFrameRate, (double)1, (double)1000))) { //max engine framerate is 1000. 
																   /*Will only loop through render and game logic if deltatimer value is higher than target frametime.
																     (capping framerates)*/
				UpdateWindowSize();
				Input::Mouse::MousePreUpdate();                    //Resets mouse deltas and booleans
				Input::InputEvents::EventUpdate();                 //Updates InputEvents logic, which recursively updates every function subscribed to an InputEvent.
				Light::ResetIDs();								   //Resets light IDs for dynamic reassignment
				Update();										   //Updaes main game logic loop
				Object::Update();								   //Updates all the individual objects one by one
				Camera::mainCamera->CameraDraw(true);              //Draws main camera after any other cameras have drawn. (This fixes possible camera desync issues.)
				PostRender();                                      //Runs after all rendering is done.
				Screenshot::PNG_RGBA8();						   //Takes a screenshot only if a screenshot has been queued. (This means when a screenshot is taken during a frame,
																   // it will take the screenshot AFTER the frame is rendered.)

				if (Camera::mainCamera == nullptr) {
					Debug::Error("Camera::mainCamera is a nullptr! Create a camera to render the scene.");
				}
				else
				{
					glfwSwapBuffers(thisWindow);				   /*Swaps front and back render buffer.
																	 (No point doing this if no camera is in use)*/
				}												  
				glfwPollEvents();								   //Polls GLFW events (Stuff related to various glfw functions)

				Input::Mouse::MouseUpdate();					   //If any changes to the mouse were made, do them now, at the end of the frame
				Input::Keyboard::SwapKeyBuffer(thisWindow);		   //Swap out key buffer (Used for determining if a key was pressed in a given frame)
				Input::Mouse::SwapButtonBuffer(thisWindow);		   //Swap out mouse button buffer (same as keyboard)
				KTYGlobal::UpdateImportantStuff(thisWindow);	   //Will update anything else important (eg. checking if user is tabbed into window)
					
				deltaTimer.Stop();								   //Stop the delta timer (for finding frametimes and deltaTime)
				Time::SetDeltaTime(deltaTimer.Value());            //Set delta time based on the value of the deltaTimer
				frameRate = (double)1 / (double)deltaTimer.Value();//Calculate framerate
				KTYGlobal::increment_frame();
				currentFrame++;                                    //increments frame self explanitory
			}
		}
		glfwTerminate();
		return 0;
	}

	void Exit() {
		glfwSetWindowShouldClose(thisWindow, 1);
	}

};
#endif