#pragma once
#include "../glIncl.h"

#define AIR_TEXTURE_LINEAR GL_LINEAR
#define AIR_TEXTURE_NEAREST GL_NEAREST
#define AIR_TEXTURE_RGB GL_RGB
#define AIR_TEXTURE_RGBA GL_RGBA

typedef GLuint		air_texture_id;

namespace air {
	struct TextureParameters {

	};
	
	struct Texture {
		Texture() {
			spritesCount = new uint32_t[32];
			for (int i = 0; i < 32; ++i) spritesCount[i] = 0;
		}

		~Texture() {
			delete[] spritesCount;
		}

		air_texture_id id = 0;
		uint32_t *spritesCount = nullptr;
	};
}