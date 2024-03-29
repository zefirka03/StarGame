#include "TextureManager.h"

namespace air {

	void TextureManager::loadTexture(const char* path, const char* name) {
		if (textures_path.find(path) == textures_path.end()) {
			int szx, szy;
			unsigned char* image = SOIL_load_image(path, &szx, &szy, 0, SOIL_LOAD_RGBA);

			GLuint id;
			glGenTextures(1, &id);

			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, szx, szy, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			Texture* tx = new Texture();
			tx->id = id;

			textures_path.insert(std::make_pair(path, tx));
			textures_names.insert(std::make_pair(name, tx));
		}
	}
	void TextureManager::loadTextureByData(char* data, TextureParameters params, const char* name) {
		GLuint id;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, params.size_x, params.size_y, 0, params.format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		Texture* tx = new Texture();
		tx->id = id;

		textures_names.insert(std::make_pair(name, tx));
	}

	TextureManager::~TextureManager() {
		for (auto it = textures_path.begin(); it != textures_path.end(); ++it) {

			delete it->second;
		}
	}

	Texture* TextureManager::getTexture(const char* name) {
		return textures_names[name];
	}
}