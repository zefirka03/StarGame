#pragma once
#include "../core/debug.h"
#include "../glIncl.h"

#include <SOIL.h>
#include <vector>
#include <map>
#include <unordered_map>

#include "Shader.h" 
	
typedef uint32_t	air_sprite_id;
typedef GLuint		air_texture_id;

constexpr air_sprite_id MAX_AVALIABLE_SPRITE_COUNT = 1500000;


namespace air {
	struct C_Camera2d;

	struct Texture {
		air_texture_id id;
		uint32_t spritesCount = 0;
	};

	struct TextureManager {
		void loadTexture(const char* path, const char* name);
		std::unordered_map<std::string, Texture*> textures_path;
		std::unordered_map<std::string, Texture*> textures_names;
		~TextureManager();
	private:
		air_texture_id _last_id = 0;
	};

	struct Transform2d {
		glm::vec3 position = glm::vec3(0);
		glm::vec2 size = glm::vec2(1);
		glm::vec2 origin = glm::vec2(0);
		glm::vec2 scale = glm::vec2(1);
		float rotation = 0;
	};

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

		Shader* shdr;
	};

	struct C_Sprite {
		Transform2d transform;

		glm::vec4 color;
		glm::vec4 textureRect;

		Texture* tex;
	};


	struct C_Camera2d {
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

		float width, height;
		Transform2d transform;

		glm::mat4 projection;
		glm::mat4 view;
	};

}