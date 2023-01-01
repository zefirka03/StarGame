#include "Input.h"

namespace air{
	bool Input::mouseWheel_Callback_inited = false;
	double Input::mouseWheel = 0;	
	glm::vec2 Input::touchPoint = glm::vec2(0);
	bool Input::clicked = false;
	bool Input::touched = false;

	bool Input::isKeyPressed(int _keycode) {
		return (glfwGetKey(Game::getInstance().getNativeWindow(), _keycode) == GLFW_PRESS);
	}
	bool Input::isKeyReleased(int _keycode) {
		return (glfwGetKey(Game::getInstance().getNativeWindow(), _keycode) == GLFW_RELEASE);
	}

	glm::vec2 Input::getTouchPos() {
		return touchPoint;
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

	bool Input::isClicked() {
		return clicked;
	}

	bool Input::isTouched() {
		return touched;
	}


	void Input::Init() {
		glfwSetScrollCallback(Game::getInstance().getNativeWindow(), wheelCallback);

#ifdef AIR_WEB
		emscripten_set_touchmove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback);
		emscripten_set_touchstart_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback);
		emscripten_set_touchend_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback);

		emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
		emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
		emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
		emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
#endif
	}

#ifdef AIR_WEB

	EM_BOOL Input::touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData)
	{
		const EmscriptenTouchPoint* t = &e->touches[0];

		if (eventType == EMSCRIPTEN_EVENT_TOUCHSTART) {
			//printf("Touch_down\n");
			Input::touchPoint.x = t->screenX;
			Input::touchPoint.y = t->screenY;

			touched = true;
		}
		if (eventType == EMSCRIPTEN_EVENT_TOUCHEND) {
			touched = false;
		}
		if (eventType == EMSCRIPTEN_EVENT_TOUCHMOVE) {
			//printf("Touch_moved\n");
			Input::touchPoint.x = t->screenX;
			Input::touchPoint.y = t->screenY;
		}
		return 0;
	}

	EM_BOOL Input::mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData)
	{
		clicked = false;
		if (eventType == EMSCRIPTEN_EVENT_CLICK) {
			//printf("Mouse_clicked\n");
			Input::touchPoint.x = e->screenX;
			Input::touchPoint.y = e->screenY;

			clicked = true;
		}

		if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN && e->buttons != 0) {
			//printf("Mouse_down\n");
			Input::touchPoint.x = e->screenX;
			Input::touchPoint.y = e->screenY;

			touched = true;
		}
		if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) {
			touched = false;
		}
		if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE && (e->movementX != 0 || e->movementY != 0)) {
			//printf("Mouse_moved\n");
			Input::touchPoint.x = e->screenX;
			Input::touchPoint.y = e->screenY;
		}

		return 0;
	}
	static inline const char* emscripten_event_type_to_string(int eventType) {
		const char* events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize",
		  "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange",
		  "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload",
		  "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "(invalid)" };
		++eventType;
		if (eventType < 0) eventType = 0;
		if (eventType >= sizeof(events) / sizeof(events[0])) eventType = sizeof(events) / sizeof(events[0]) - 1;
		return events[eventType];
	}

	EM_BOOL Input::touchMoveCallback(int eventType, const EmscriptenTouchEvent* e, void* userData) {
		const EmscriptenTouchPoint* t = &e->touches[0];
		touchPoint.x = t->screenX;
		touchPoint.y = t->screenY;
		printf("Mobile: %f %f \n", touchPoint.x, touchPoint.y);
	}
	EM_BOOL Input::mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
		printf("%s, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), target: (%ld, %ld)\n",
			emscripten_event_type_to_string(eventType), e->screenX, e->screenY, e->clientX, e->clientY,
			e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "",
			e->button, e->buttons, e->movementX, e->movementY, e->targetX, e->targetY);

		//if (e->screenX != 0 && e->screenY != 0 && e->clientX != 0 && e->clientY != 0 && e->targetX != 0 && e->targetY != 0)
		//{
		//	if (eventType == EMSCRIPTEN_EVENT_CLICK) gotClick = 1;
		//	if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN && e->buttons != 0) gotMouseDown = 1;
		//	if (eventType == EMSCRIPTEN_EVENT_DBLCLICK) gotDblClick = 1;
		//	if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) gotMouseUp = 1;
		//	if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE && (e->movementX != 0 || e->movementY != 0)) gotMouseMove = 1;
		//}

		if (eventType == EMSCRIPTEN_EVENT_CLICK && e->screenX == -500000)
		{
			printf("ERROR! Received an event to a callback that should have been unregistered!\n");
		}
	}
#endif

}