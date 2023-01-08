#pragma once
#include "../core/debug.h"
#include "../glIncl.h"
#include "../core/Game.h"

#include "Framebuffer.h"
#include "IRenderer.h"

namespace air {
	
	struct SpriteInstanceWeb {
		glm::vec3 position;
		glm::vec4 color;

		glm::vec2 uv;
		Texture* tex;
		uint8_t layer;
	};

	struct quadInstanceWeb {
		SpriteInstanceWeb verts[6];
	};

	class Renderer2d_Web : public IRenderer{
	public:
		Renderer2d_Web(air_sprite_id _sprite_count);

		void draw(const SpriteInstance& _vert) override;
		void submit(Camera2d& cam, C_RenderTexture* rendTex = nullptr) override;
		void clear() override;

		void prepare() override;
		~Renderer2d_Web();
	private:
		static bool texture_sort_comparator(quadInstanceWeb const& a, quadInstanceWeb const& b) {
			if (a.verts[0].layer == b.verts[0].layer) {
				if(a.verts[0].tex == b.verts[0].tex)
					return a.verts[0].position.z < b.verts[0].position.z;
				else return a.verts[0].tex < b.verts[0].tex;
			}
			else return a.verts[0].layer > b.verts[0].layer;
		}

		quadInstanceWeb* drawQueue;
		size_t draw_it;
	};

}

