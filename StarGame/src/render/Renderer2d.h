#pragma once
#include "../core/debug.h"
#include "../glIncl.h"

#include <vector>

#include "Shader.h" 
#include "Camera2d.h"
	
typedef uint32_t air_sprite_id;

constexpr air_sprite_id MAX_AVALIABLE_SPRITE_COUNT = 1500000;

namespace air {
	class Renderer2d {
		struct SpriteInstance;
	public:
		Renderer2d(air_sprite_id _sprite_count);
		void draw(const SpriteInstance& _vert);
		void submit(Camera2d& cam);
		~Renderer2d();
	private:
		struct SpriteInstance {
			glm::vec2 pos = {};
			glm::vec2 size = {};
			glm::vec4 color = glm::vec4(1);
		};

		SpriteInstance* drawQueue;
		//last id in drawQueue
		size_t draw_it;

		air_sprite_id maxSpriteCount;

		GLuint vbo_id;
		GLuint vao_id;

		Shader* shdr;
	};

	struct C_Sprite {
		glm::vec2 position = {0, 0};
		glm::vec2 size = {100, 100};

		glm::vec4 color = {1, 1, 1, 1};
	};
}