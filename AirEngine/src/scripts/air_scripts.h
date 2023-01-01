#pragma once

#include "../glIncl.h"
#include "../ecs/air_ecs.h"
#include "../ecs/Script.h"

namespace air {
	class S_Camera2dController : public Script {
	public:
		void OnUpdate(float deltaTime);
		void OnCreate();
	private:
		C_Camera2d* cam_instance = nullptr;

		bool start_move = false;

		glm::vec2 moved = glm::vec2(0);
		glm::vec2 prev_m = glm::vec2(0);
		glm::vec2 delta_m = glm::vec2(0);
	};
}