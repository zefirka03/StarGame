#include "Input.h"

namespace air{
	bool Input::mouseWheel_Callback_inited = false;
	double Input::mouseWheel = 0;

	bool Input::isKeyPressed(int _keycode) {
		return glfwGetKey(Game::getInstance().getNativeWindow(), _keycode);
	}
	glm::vec2 Input::getCursorPos() {
		double x, y;
		glfwGetCursorPos(Game::getInstance().getNativeWindow(), &x, &y);
		return glm::vec2(x, y);
	}

	bool Input::isMousePressed_Left() {
		return (glfwGetMouseButton(Game::getInstance().getNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	}

	bool Input::isMouseRelease_Left() {
		return (glfwGetMouseButton(Game::getInstance().getNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE);
	}

	double Input::getMouseWheel() {
		if (!mouseWheel_Callback_inited) {
			glfwSetScrollCallback(Game::getInstance().getNativeWindow(), wheelCallback);
			mouseWheel_Callback_inited = true;
		}
		return mouseWheel;
	}

	void Input::pollEvents() {
		mouseWheel = 0;
		glfwPollEvents();
	}

	void Input::wheelCallback(GLFWwindow* win, double x, double y) {
		mouseWheel = y;
	}
}