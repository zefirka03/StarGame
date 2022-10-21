#include "air_scripts.h"
#include "../core/Input.h"

namespace air {
	void S_Camera2dController::OnUpdate(float deltaTime) {
		if (Input::isMousePressed_Left()) {
			mouse_clicked = true;

			delta_m = Input::getCursorPos() - prev_m;
			prev_m = Input::getCursorPos();

			cam_instance->move({ (glm::vec2(-1.f) / cam_instance->transform.scale) * delta_m, 0 });
		}
		else prev_m = Input::getCursorPos();

		scale_factor += Input::getMouseWheel() / 10.f;

		double w = Input::getMouseWheel();
		if (w > 0)
			cam_instance->scale(pow(11.f/10.f, w));
		else if(w<0) cam_instance->scale(pow(10.f / 11.f, -w));
	}

	void S_Camera2dController::OnCreate() {
		cam_instance = &getComponent<C_Camera2d>();
		cam_instance->transform.origin = glm::vec2(cam_instance->width / 2.f, cam_instance->height / 2.f);
	}
}