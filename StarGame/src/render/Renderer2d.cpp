#include "Renderer2d.h"

namespace air {
	void TextureManager::loadTexture(const char* path, const char* name){
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

			Texture* tx = new Texture{ id, 0 };

			textures_path.insert(std::make_pair(path, tx));
			textures_names.insert(std::make_pair(name, tx));
		}
	}


	TextureManager::~TextureManager() {
		for (auto it = textures_path.begin(); it != textures_path.end(); ++it) {
			glDeleteTextures(1, &it->second->id);
			delete it->second;
		}
	}

	



	Renderer2d::Renderer2d(air_sprite_id _sprite_count) {
		assert(_sprite_count < MAX_AVALIABLE_SPRITE_COUNT && "You cant to store so many sprites!");
		maxSpriteCount = _sprite_count;

		drawQueue = new SpriteInstance[_sprite_count];
		draw_it = 0;

		shdr = new Shader("src/render/shaders/shader.shader", AIR_SHADER_VGF);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteInstance) * maxSpriteCount, nullptr, GL_DYNAMIC_DRAW);

		std::cout << sizeof(SpriteInstance) * maxSpriteCount << " video bites allocated\n";

		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 5));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 7));
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 9));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 10));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 14));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//add an instance to draw queue
	void Renderer2d::draw(const SpriteInstance& _vert) {
		if (draw_it >= maxSpriteCount) {
			WA("Renderer2d: You are trying to render more than maximum allowed sprites!");
			return;
		}

		_vert.tex->spritesCount++;

		drawQueue[draw_it++] = _vert;
	}



	//render all objects in draw queue and clear queue
	void Renderer2d::submit(C_Camera2d& cam) {
		std::sort(drawQueue, drawQueue + draw_it, texture_sort_comparator);

		shdr->setMatrix4f(cam.getProjection(), "proj");
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, draw_it * sizeof(SpriteInstance), drawQueue);
		glBindVertexArray(vao_id);

		int j = 0;
		while (j < draw_it) {
			auto it = (drawQueue + j);

			glBindTexture(GL_TEXTURE_2D, it->tex->id);

			shdr->use();
			glDrawArrays(GL_POINTS, j, it->tex->spritesCount);
			shdr->unuse();

			j += it->tex->spritesCount;
			it->tex->spritesCount = 0;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		draw_it = 0;
	}

	Renderer2d::~Renderer2d() {
		glDeleteBuffers(1, &vbo_id);
		glDeleteVertexArrays(1, &vao_id);
		delete shdr;
		delete[] drawQueue;
	}
}