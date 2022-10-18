#pragma once
#include "Game.h"

namespace air {
	class Input {
	public:
		static bool isKeyPressed(int _keycode);
		static glm::vec2 getCursorPos();

		static bool isMousePressed_Left();

		static bool isMouseRelease_Left();

		static double getMouseWheel();

		static void pollEvents();

	private:
		static void wheelCallback(GLFWwindow* win, double x, double y);

		static double mouseWheel;
		static bool mouseWheel_Callback_inited;
	};
}