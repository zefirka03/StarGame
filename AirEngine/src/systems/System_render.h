#pragma once

#include "../ecs/air_ecs.h"

#include "../render/Renderer2d.h"
#include "../components/Transform.h"

namespace air {

	struct SystemRender_States {
		uint32_t sprites = 0;
	};

	class _System_Render : public System {
	public:
		_System_Render(air_sprite_id sprite_count);
		void init() override;
		void update(float _deltaTime) override;
		void terminate() override;
		SystemRender_States getStates();

		~_System_Render();
	private:
		Renderer2d* render;
		SystemRender_States states;
	};
}