#pragma once
#include "main.h"
#include "KTYKeyboard.h"
#include "KTYMouse.h"
#include "Logger.h"
#include <functional>

namespace Input {

	class InputEvent {
	private:
		//_value is a float to allow analog inputs.
		float _value;
		std::vector<std::function<void(float)>> pressFuncs;
		std::vector<std::function<void(float)>> pressOnFrameFuncs;
		std::vector<std::function<void(float)>> depressOnFrameFuncs;
	public: 

		/// <summary>
		/// Updates all subscribed functions.
		/// </summary>
		void SubEventUpdate() {
			for (int kcode : keycodeActions) {
				if (Keyboard::Key(pressed, kcode)) {
					for (int i = 0; i < pressFuncs.size(); i++) {
						pressFuncs[i](_value);
					}
				}
				if (Keyboard::Key(pressed_this_frame, kcode)) {
					for (int i = 0; i < pressOnFrameFuncs.size(); i++) {
						pressOnFrameFuncs[i](_value);
					}
				}
				if (Keyboard::Key(depressed_this_frame, kcode)) {
					for (int i = 0; i < depressOnFrameFuncs.size(); i++) {
						depressOnFrameFuncs[i](_value);
					}
				}
			}
		}
		std::vector<int> keycodeActions;
		std::string actionName;
		/// <summary>
		/// Returns InputEvent value. 
		/// </summary>
		/// <typeparam name="Type"></typeparam>
		/// <returns></returns>
		template<typename T>
		T GetVal() {
			std::type_info type_id = typeid(T);
			switch (type_id) {
			case typeid(bool) :
				if (_value <= 0)
					return false;
				else
					return true;
				break;
			case typeid(float) :
				return _value;
				break;
			default:
				return _value;
				break;
			}
		}
		/// <summary>
		/// Subscribes a function such as "void Jump(float value)" to either "pressed", "pressed_this_frame", or "depressed_this_frame".
		/// </summary>
		/// <param name="functionType"></param>
		/// <param name="func"></param>
		void RegisterFunc(int functionType, void(*func)(float)) {
			switch (functionType) {
			case pressed:
				pressFuncs.push_back(func);
				break;

			case pressed_this_frame:
				pressOnFrameFuncs.push_back(func);
				break;

			case depressed_this_frame:
				depressOnFrameFuncs.push_back(func);
				break;

			default:
				pressFuncs.push_back(func);
				break;
			}
		}

	};
	class InputEvents {
	private:
		static std::vector<InputEvent> events;
		static DebugLogger ieLogger;
		/// <summary>
		/// Returns the index to an event in the events vector. 
		/// </summary>
		/// <param name="action"></param>
		/// <param name="log"></param>
		/// <returns></returns>
		static int GetEventIndex(std::string action, bool log = true) {
			for (int i = 0; i < events.size(); i++) {
				if (events[i].actionName == action) {
					return i;
				}
			}
			if (log)
				ieLogger.Error("Event '" + action + "' not found. nullptr returned.");
			return -1;
		}
	public:
		static void EventUpdate() {
			for (InputEvent ie : events) {
				ie.SubEventUpdate(); 
			}
		}
		static void Init() {
			ieLogger.defaultColour = DBG_BLUE;
			ieLogger.LoggerName = "INPUTEVENT";
			ieLogger.CLog("InputEvent System active.");
		}
		/// <summary>
		/// Returns a pointer to the event if found with the provided action name.
		/// </summary>
		/// <param name="action"></param>
		/// <param name="log"></param>
		/// <returns></returns>
		static InputEvent* GetEvent(std::string action, bool log = true) {
			for (int i = 0; i < events.size(); i++) {
				if (events[i].actionName == action) {
					return &events[i];
				}
			}
			if (log)
				ieLogger.Error("Event '" + action + "' not found. nullptr returned.");
			return nullptr;
		}
		/// <summary>
		/// Registers an event in the event system with an action name and a single keycode. Multiple keycodes can also be registered by replacing "keycode" with a vector of keycodes (format:int).
		/// </summary>
		/// <param name="action"></param>
		/// <param name="keycode"></param>
		/// <param name="log"></param>
		static InputEvent* RegisterEvent(std::string action, int keycode, bool log = true) {
			return RegisterEvent(action, std::vector<int>{keycode}, log);
		}
		/// <summary>
		/// Register an event in the event system with an action name and a vector of keycodes.
		/// </summary>
		/// <param name="action"></param>
		/// <param name="keycodes"></param>
		/// <param name="log"></param>
		static InputEvent* RegisterEvent(std::string action, std::vector<int> keycodes, bool log = true) {
			InputEvent* ie = GetEvent(action, false);
			if (ie != nullptr) { // action already exists
				if (log)
					ieLogger.Warning("Event " + action + " is already registered.");
				return ie;
			}
			InputEvent ie_new;
			ie_new.actionName = action;
			ie_new.keycodeActions = keycodes;
			events.push_back(ie_new);
			return &events[events.size() - 1];
		}
		/// <summary>
		/// Unregisters event given action name.
		/// </summary>
		/// <param name="action"></param>
		/// <param name="log"></param>
		static void UnregisterEvent(std::string action, bool log = true) {
			int eventIndex = GetEventIndex(action, false);
			if (eventIndex == -1) { // action doesn't exist, disregard
				if (log)
					ieLogger.Warning("No event by name " + action + " is registered.");
				return;
			}
			events.erase(events.begin() + eventIndex);
		}
	};
}