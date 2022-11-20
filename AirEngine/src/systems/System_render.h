#pragma once

#include "../ecs/air_ecs.h"

#include "../render/Renderer2d.h"
#include "../components/Transform.h"

namespace air {
	class _System_Render : public System {
	public:
		_System_Render(air_sprite_id sprite_count);
		void init() override;
		void update(float _deltaTime) override;
		void terminate() override;
		~_System_Render();
	private:
		Renderer2d* render;
	};
}