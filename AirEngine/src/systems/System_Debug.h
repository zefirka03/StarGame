#pragma once

#include "../ecs/air_ecs.h"

#include "../render/Renderer2d.h"
#include "../components/Transform.h"

namespace air {

	class _System_Debug : public System {
		struct Line {
			glm::vec2 p1;
			glm::vec2 p2;
			glm::vec4 color;
		};
	public:
		_System_Debug(air_sprite_id sprite_count);

		void drawLine(glm::vec2 _p1, glm::vec2 _p2, glm::vec4 _color = glm::vec4(1));
		void drawQuad(glm::vec2 _pos, glm::vec2 _size, glm::vec4 _color = glm::vec4(1), glm::vec2 _origin = glm::vec2(0));

		~_System_Debug();

	private:
		void init() override;
		void update(float _deltaTime) override;
		void terminate() override;

		std::vector<Line> lines;
		RendererDebug* render;
	};
}