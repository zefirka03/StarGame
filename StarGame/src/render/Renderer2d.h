#pragma once
#include "../core/debug.h"
#include "../glIncl.h"

#include <vector>
#include <map>
#include <unordered_map>

#include "Shader.h" 
#include "TextureManager.h"
#include "../ecs/System.h"

#include "../components/C_Transform2d.h"
#include "../ecs/Component.h"
	
typedef uint32_t	air_sprite_id;

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
		static bool texture_sort_comparator(SpriteInstance const& a, SpriteInstance const& b) {
			if (a.transform.position.z == b.transform.position.z)
				return a.tex < b.tex;
			else return a.transform.position.z < b.transform.position.z;
		}

		struct SpriteInstance {
			Transform2d transform;

			glm::vec4 color;
			glm::vec4 textureRect;

			Texture* tex;
		};
		
		SpriteInstance* drawQueue;
		//last id in drawQueue
		size_t draw_it;

		air_sprite_id maxSpriteCount;

		GLuint vbo_id;
		GLuint vao_id;

		Shader* spriteShader;
	};

	class Renderer2dRectangles {
		struct RectangleInstance;
	public:
		Renderer2dRectangles(air_sprite_id _rectangles_count);
		void draw(const RectangleInstance& _vert);

		void submit(C_Camera2d& cam);
		~Renderer2dRectangles();
	private:
		static bool sort_comparator(RectangleInstance const& a, RectangleInstance const& b) {
			return a.transform.position.z < b.transform.position.z;
		}

		struct RectangleInstance {
			Transform2d transform;

			glm::vec4 color;
			float borderThickness;
		};

		RectangleInstance* drawQueue;
		//last id in drawQueue
		size_t draw_it;

		air_sprite_id maxRectanglesCount;

		GLuint vbo_id;
		GLuint vao_id;

		Shader* rectangleShader;
	};




	struct C_Camera2d : public Component {
		C_Camera2d() = default;
		C_Camera2d(float _w, float _h) : width(_w), height(_h) {
			width = _w;
			height = _h;

			transform.size = glm::vec2(_w, _h);

			projection = glm::ortho(0.f, _w, _h, 0.f);
			view = glm::mat4(1);

			this->transform.position = glm::vec3(0);
			this->transform.origin = glm::vec2(0, 0);
			this->transform.scale = glm::vec2(1);
		};
		glm::mat4 getProjection() {
			view = glm::translate(glm::mat4(1), glm::vec3(transform.origin, 0));
			view = glm::scale(view, glm::vec3(transform.scale, 0));
			view = glm::translate(view, -transform.position);
			return projection * view;
		}
		void move(glm::vec3 _factor) {
			transform.position += _factor;
		}
		void scale(float _factor) {
			transform.scale *= _factor;
		}

		float width = 100, height = 100;
		Transform2d transform;

		glm::mat4 projection = glm::mat4(0);
		glm::mat4 view = glm::mat4(0);
	};


	class _System_Render : public System {
	public:
		_System_Render(air_sprite_id sprite_count) {
			render = new Renderer2d(sprite_count);
		}
		void init() override {
		}
		void update(float _deltaTime) override {

			C_Camera2d* main_camera = nullptr;

			reg->view<C_Camera2d>().each([&](auto& cam) {
				main_camera = &cam;
			});
				
			auto reg_render = reg->view<C_Sprite, C_Transform2d>();

			reg_render.each([&](C_Sprite& sprite, C_Transform2d& transform) {

				render->draw({ transform.transform, sprite.color, sprite.textureRect, sprite.tex });
			});

			render->submit(*main_camera);
		}
		void terminate() override {
			delete render;
		}
		~_System_Render() {
			delete render;
		}
	private:
		Renderer2d* render;
	};

}