#pragma once
#include "../glIncl.h"

#define AIR_TEXTURE_LINEAR GL_LINEAR
#define AIR_TEXTURE_NEAREST GL_NEAREST
#define AIR_TEXTURE_RGB GL_RGB
#define AIR_TEXTURE_RGBA GL_RGBA
#define AIR_TEXTURE_R8 GL_R8
#define AIR_TEXTURE_RED GL_RED

typedef GLuint		air_texture_id;

namespace air {
	struct TextureParameters {
		int size_x = 100;
		int size_y = 100;
		uint16_t internalFormat = AIR_TEXTURE_RGBA;
		uint16_t format = AIR_TEXTURE_RGBA;
	};
	
	struct Texture {
		Texture() {
			spritesCount = new uint32_t[32];
			for (int i = 0; i < 32; ++i) spritesCount[i] = 0;
		}

		~Texture() {
			if (spritesCount) {
				delete[] spritesCount;
				spritesCount = nullptr;
			}
			glDeleteTextures(1, &id);
		}

		air_texture_id id = 0;
		uint32_t *spritesCount = nullptr;
	};

}