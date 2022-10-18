#pragma once
#include "../core/debug.h"
#include "../glIncl.h"

#include <vector>

#include "Shader.h" 
	
typedef uint32_t air_sprite_id;

constexpr air_sprite_id MAX_AVALIABLE_SPRITE_COUNT = 1500000;

namespace air {
	struct C_Camera2d;

	class Renderer2d {
		struct SpriteInstance;
	public:
		Renderer2d(air_sprite_id _sprite_count);
		void draw(const SpriteInstance& _vert);
		void submit(C_Camera2d& cam);
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

	struct Transform2d {
		glm::vec2 position;
		glm::vec2 origin;
		glm::vec2 scale;
		glm::vec2 size;
	};

	struct C_Camera2d {
		C_Camera2d() = default;
		C_Camera2d(float _w, float _h) : width(_w), height(_h) {
			width = _w;
			height = _h;

			transform.size = glm::vec2(_w, _h);

			projection = glm::ortho(0.f, _w, _h, 0.f);
			view = glm::mat4(1);

			this->transform.position = glm::vec2(0, 0);
			this->transform.origin = glm::vec2(0, 0);
			this->transform.scale = glm::vec2(1);
		};
		glm::mat4 getProjection() {
			view = glm::translate(glm::mat4(1), glm::vec3(transform.origin, 0));
			view = glm::scale(view, glm::vec3(transform.scale, 0));
			view = glm::translate(view, -glm::vec3(transform.position, 0));
			return projection * view;
		}
		void move(glm::vec2 _factor) {
			transform.position += _factor;
		}
		void scale(float _factor) {
			transform.scale *= _factor;
		}

		float width, height;
		Transform2d transform;

		glm::mat4 projection;
		glm::mat4 view;
	};

}