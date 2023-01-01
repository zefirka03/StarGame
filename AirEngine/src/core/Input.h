#pragma once
#include "Game.h"
#include "../components/Camera2d.h"

namespace air {
	class Input {
	public:
		static bool isKeyPressed(int _keycode);
		static bool isKeyReleased(int _keycode);
		static glm::vec2 getCursorPos();
		static glm::vec2 getCursorPos(Camera2d const& _camera);

		static glm::vec2 getTouchPos();
		static bool isClicked();

		static bool isTouched();

		static bool isMousePressed_Left();

		static bool isMouseRelease_Left();

		static bool isMousePressed_Right();

		static bool isMouseRelease_Right();

		static double getMouseWheel();

		static void pollEvents();

		static void Init();

	private:
		static void wheelCallback(GLFWwindow* win, double x, double y);

#ifdef AIR_WEB
		static EM_BOOL touchMoveCallback(int eventType, const EmscriptenTouchEvent* e, void* userData);
		static EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData);
		static EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData);
		static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
#endif

		static glm::vec2 touchPoint;

		static double mouseWheel;
		static bool clicked;

		static bool touched;

		static bool mouseWheel_Callback_inited;
		static bool mouseMove_Callback_inited;
		static bool touchMove_Callback_inited;
	};
}