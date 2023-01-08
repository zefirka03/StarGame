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
#include "IRenderer.h"
	

namespace air {

	class Renderer2d : public IRenderer {
	public:
		Renderer2d(air_sprite_id _sprite_count);
		void draw(const SpriteInstance& _vert) override;
		void submit(Camera2d& cam, C_RenderTexture* rendTex = nullptr) override;
		void clear() override;

		void prepare() override;
		~Renderer2d();

	private:
		static bool texture_sort_comparator(SpriteInstance const& a, SpriteInstance const& b) {
			if (a.layer == b.layer) {
				if (a.transform.position.z == b.transform.position.z)
					return a.tex < b.tex;
				else return a.transform.position.z < b.transform.position.z;
			}
			else return a.layer > b.layer;
		}

		
		SpriteInstance* drawQueue;
		//last id in drawQueue
		size_t draw_it;
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