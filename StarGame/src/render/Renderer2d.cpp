#include "Renderer2d.h"

namespace air {
	Renderer2d::Renderer2d(air_sprite_id _sprite_count) {
		assert(_sprite_count < MAX_AVALIABLE_SPRITE_COUNT && "You cant to store so many sprites!");
		maxSpriteCount = _sprite_count;

		drawQueue = new SpriteInstance[_sprite_count];
		draw_it = 0;

		shdr = new Shader("src/render/shaders/shader.shader", AIR_SHADER_VGF);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteInstance) * maxSpriteCount, nullptr, GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 2));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInstance), (GLvoid*)(sizeof(GLfloat) * 4));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//add an instance to draw queue
	void Renderer2d::draw(const SpriteInstance& _vert) {
		if (draw_it >= maxSpriteCount) {
			WA("Renderer2d: You are trying to render more than maximum allowed sprites!");
			return;
		}
		drawQueue[draw_it++] = _vert;
	}

	//render all objects in draw queue and clear queue
	void Renderer2d::submit(C_Camera2d& cam) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, draw_it * sizeof(SpriteInstance), drawQueue);

		glBindVertexArray(vao_id);

		shdr->setMatrix4f(cam.getProjection(), "proj");
		shdr->use();
		glDrawArrays(GL_POINTS, 0, draw_it);
		shdr->unuse();

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