#include "Renderer2d.h"

namespace air {

	Renderer2dRectangles::Renderer2dRectangles(air_sprite_id _rectangles_count) {
		assert(_rectangles_count < MAX_AVALIABLE_SPRITE_COUNT && "You cant to store so many sprites!");
		maxRectanglesCount = _rectangles_count;

		drawQueue = new RectangleInstance[_rectangles_count];
		draw_it = 0;

		rectangleShader = new Shader("src/render/shaders/RectangleShader.shader", AIR_SHADER_VGF);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RectangleInstance) * maxRectanglesCount, nullptr, GL_DYNAMIC_DRAW);

		std::cout << sizeof(RectangleInstance) * maxRectanglesCount << " video bites allocated\n";

		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 5));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 7));
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 9));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 10));
		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(RectangleInstance), (GLvoid*)(sizeof(GLfloat) * 14));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//add an instance to draw queue
	void Renderer2dRectangles::draw(const RectangleInstance& _vert) {
		if (draw_it >= maxRectanglesCount) {
			WA("Renderer2d: You are trying to render more than maximum allowed sprites!");
			return;
		}

		drawQueue[draw_it++] = _vert;
	}



	//render all objects in draw queue and clear queue
	void Renderer2dRectangles::submit(C_Camera2d& cam) {
		std::sort(drawQueue, drawQueue + draw_it, sort_comparator);

		rectangleShader->setMatrix4f(cam.getProjection(), "proj");

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, draw_it * sizeof(RectangleInstance), drawQueue);
		glBindVertexArray(vao_id);

		rectangleShader->use();
		glDrawArrays(GL_POINTS, 0, draw_it);
		rectangleShader->unuse();

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		draw_it = 0;
	}

	Renderer2dRectangles::~Renderer2dRectangles() {
		glDeleteBuffers(1, &vbo_id);
		glDeleteVertexArrays(1, &vao_id);

		delete rectangleShader;
		delete[] drawQueue;
	}
}