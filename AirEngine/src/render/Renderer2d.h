#pragma once
#include "../core/debug.h"
#include "../glIncl.h"

#include <vector>
#include <map>
#include <unordered_map>

#include "../core/Game.h" 

#include "Shader.h" 
#include "TextureManager.h"
#include "../ecs/System.h"

#include "../components/Transform.h"
#include "../components/Camera2d.h"
#include "../ecs/air_ecs.h"
#include "Framebuffer.h"
	
typedef uint32_t	air_sprite_id;

constexpr air_sprite_id MAX_AVALIABLE_SPRITE_COUNT = 1500000;

namespace air {

	class Renderer2d {
		struct SpriteInstance;
	public:
		Renderer2d(air_sprite_id _sprite_count);
		void draw(const SpriteInstance& _vert);

		void submit(Camera2d& cam, C_RenderTexture* rendTex = nullptr);

		void clear();
		~Renderer2d();

		size_t getLastDrawCount();

	private:
		static bool texture_sort_comparator(SpriteInstance const& a, SpriteInstance const& b) {
			if (a.layer == b.layer) {
				if (a.transform.position.z == b.transform.position.z)
					return a.tex < b.tex;
				else return a.transform.position.z < b.transform.position.z;
			}
			else return a.layer > b.layer;
		}

		struct SpriteInstance {
			Transform2d transform;

			glm::vec4 color;
			glm::vec4 textureRect;

			Texture* tex;
			uint8_t layer;
		};
		
		SpriteInstance* drawQueue;
		//last id in drawQueue
		size_t draw_it;

		air_sprite_id maxSpriteCount;

		GLuint vbo_id;
		GLuint vao_id;

		Shader* spriteShader;

		size_t last_draw_count=0;
	};

	class Renderer2dRectangles {
		struct RectangleInstance;
	public:
		Renderer2dRectangles(air_sprite_id _rectangles_count);
		void draw(const RectangleInstance& _vert);

		void submit(Camera2d& cam);
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

	class RendererDebug {
	public:
		struct LineInstance;
	public:
		struct LineInstance {
			glm::vec3 pos;
			glm::vec4 color;
		};

		RendererDebug(air_sprite_id _objects_count);
		void drawLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color = glm::vec4(1));
		void drawQuad(Transform2d const& transform, glm::vec4 color = glm::vec4(1));

		void submit(Camera2d& cam);
		~RendererDebug();
	private:
		static bool sort_comparator(LineInstance const& a, LineInstance const& b) {
			return a.pos.z < b.pos.z;
		}


		LineInstance* drawQueue;
		//last id in drawQueue
		size_t draw_it;

		air_sprite_id maxCount;

		GLuint vbo_id;
		GLuint vao_id;

		Shader* debugShader;
	};


	
}