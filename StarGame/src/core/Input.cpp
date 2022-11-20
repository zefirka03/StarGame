#include "Input.h"

namespace air{
	bool Input::mouseWheel_Callback_inited = false;
	double Input::mouseWheel = 0;

	bool Input::isKeyPressed(int _keycode) {
		return (glfwGetKey(Game::getInstance().getNativeWindow(), _keycode) == GLFW_PRESS);
	}
	bool Input::isKeyReleased(int _keycode) {
		return (glfwGetKey(Game::getInstance().getNativeWindow(), _keycode) == GLFW_RELEASE);
	}
	glm::vec2 Input::getCursorPos() {
		double x, y;
		glfwGetCursorPos(Game::getInstance().getNativeWindow(), &x, &y);
		return glm::vec2(x, y);
	}

	 glm::vec2 Input::getCursorPos(Camera2d const& _camera) {
		double x, y;
		glfwGetCursorPos(Game::getInstance().getNativeWindow(), &x, &y);

		int w_x, w_y;
		glfwGetWindowSize(Game::getInstance().getNativeWindow(), &w_x, &w_y);
		
		y = w_y - y;

		glm::vec4 out = glm::inverse(_camera.getProjection()) * glm::inverse(_camera.getView()) * glm::vec4((glm::vec2(2*x, 2*y))/glm::vec2(_camera.width, _camera.height) , 0, 0);

		return	glm::vec2(_camera.transform.position.x, _camera.transform.position.y) -
				glm::vec2(_camera.transform.origin.x, _camera.transform.origin.y) / _camera.transform.scale +
				glm::vec2{out.x, out.y};
	}

	bool Input::isMousePressed_Left() {
		return (glfwGetMouseButton(Game::getInstance().getNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	}

	bool Input::isMouseRelease_Left() {
		return (glfwGetMouseButton(Game::getInstance().getNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE);
	}

	bool Input::isMousePressed_Right() {
		return (glfwGetMouseButton(Game::getInstance().getNativeWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	}

	bool Input::isMouseRelease_Right() {
		return (glfwGetMouseButton(Game::getInstance().getNativeWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE);
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