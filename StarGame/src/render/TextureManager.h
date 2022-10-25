#pragma once
#include "../glIncl.h"

#include <unordered_map>

typedef GLuint		air_texture_id;

namespace air {
	struct Texture {
		air_texture_id id;
		uint32_t spritesCount = 0;
	};

	//You can bind the texture by path the name and then you can reques that texture*
	struct TextureManager {
		void loadTexture(const char* path, const char* name);
		Texture* getTexture(const char* name);
		~TextureManager();
	private:
		std::unordered_map<std::string, Texture*> textures_path;
		std::unordered_map<std::string, Texture*> textures_names;
		air_texture_id _last_id = 0;
	};

}