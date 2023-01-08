#include "air_scripts.h"
#include "../core/Input.h"

namespace air {
	void S_Camera2dController::OnUpdate(float deltaTime) {

			//std::cout << nPos.x << " " << nPos.y << '\n';
			//std::cout << cam_instance->transform.size.x << " " << cam_instance->transform.size.y << '\n';
		if (Input::isTouched()) {
			glm::vec2 nPos = Input::getTouchPos();

			//printf("%f %f \n", nPos.x, nPos.y);
			if (!start_move) {
				prev_m = nPos;
				start_move = true;
			}

			delta_m = nPos - prev_m;
			prev_m = nPos;
			moved = glm::vec4(glm::vec2(-1.f, 1.f) * delta_m / cam_instance->camera.transform.scale, 0, 0);
			cam_instance->camera.move(glm::vec3(moved.x, moved.y, 0)) ;	
			//std::cout << nPos.x << " " << nPos.y<<'\n';
		}
		else prev_m = glm::vec2(0), start_move = false;

		double w = Input::getMouseWheel();
		if (w > 0)
			cam_instance->camera.scale(float(pow(11.f/10.f, w)));
		else if(w<0) cam_instance->camera.scale(float(pow(10.f / 11.f, -w)));

		//printf("cam: %f %f %f\n", cam_instance->camera.transform.position.x, cam_instance->camera.transform.position.y, cam_instance->camera.transform.position.z);
	}

	void S_Camera2dController::OnCreate() {
		cam_instance = &getComponent<C_Camera2d>();
		cam_instance->camera.transform.origin = glm::vec2(cam_instance->camera.width / 2.f, cam_instance->camera.height / 2.f);
	}
}