#pragma once
#include "../glIncl.h"

#include "../components/Camera2d.h"

#include "Framebuffer.h"
#include "Shader.h"

typedef uint32_t	air_sprite_id;

constexpr air_sprite_id MAX_AVALIABLE_SPRITE_COUNT = 1500000;

namespace air {
	struct SpriteInstance {
		Transform2d transform;

		glm::vec4 color;
		glm::vec4 textureRect;

		Texture* tex;
		uint8_t layer;
	};

	struct RenderStats {
		size_t last_draw_count = 0;
	};

	class IRenderer {
	public:
		virtual void draw(const SpriteInstance& _vert) = 0;
		virtual void submit(Camera2d& cam, C_RenderTexture* rendTex = nullptr) = 0;
		virtual void clear() = 0;

		RenderStats getStats() {
			return stats;
		}
	protected:
		RenderStats stats;

		air_sprite_id maxSpriteCount;

		GLuint vbo_id;
		GLuint vao_id;

		Shader* spriteShader;
	};
}