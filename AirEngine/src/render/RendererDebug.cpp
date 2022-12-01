#include "Renderer2d.h"

namespace air {
	RendererDebug::RendererDebug(air_sprite_id _objects_count) {
		assert(_objects_count < MAX_AVALIABLE_SPRITE_COUNT && "You cant to store so many sprites!");
		maxCount = _objects_count;

		drawQueue = new LineInstance[_objects_count];
		draw_it = 0;

		debugShader = new Shader();
		const char* shader =
			#include "shaders/_debugRendererShader.shader"
		;
		debugShader->loadFromString(
			shader, 
			AIR_SHADER_VF
		);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(LineInstance) * maxCount, nullptr, GL_DYNAMIC_DRAW);

		//std::cout << sizeof(LineInstance) * maxCount << " video bites allocated\n";
		std::cout << sizeof(LineInstance) * _objects_count << " video bites allocated\n";

		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineInstance), (GLvoid*)(sizeof(GLfloat) * 0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineInstance), (GLvoid*)(sizeof(GLfloat) * 3));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//add an instance to draw queue
	void RendererDebug::drawLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color) {
		if (draw_it >= maxCount) {
			WA("Renderer2d: You are trying to render more than maximum allowed sprites!");
			return;
		}

		drawQueue[draw_it++] = LineInstance{ pos1, color };
		drawQueue[draw_it++] = LineInstance{ pos2, color };
	}


	void RendererDebug::drawQuad(Transform2d const& transform, glm::vec4 color) {
		glm::mat4 view = glm::rotate(glm::mat4(1), -transform.rotation, glm::vec3(0,0,1));
		
		glm::vec4 pos1 = glm::vec4(transform.position, 1) + view * glm::vec4(0 - transform.origin.x, 0 - transform.origin.y, 0, 1);
		glm::vec4 pos2 = glm::vec4(transform.position, 1) + view * glm::vec4(transform.size.x - transform.origin.x, 0 - transform.origin.y, 0, 1);
		glm::vec4 pos3 = glm::vec4(transform.position, 1) + view * glm::vec4(transform.size.x - transform.origin.x, transform.size.y - transform.origin.y, 0, 1);
		glm::vec4 pos4 = glm::vec4(transform.position, 1) + view * glm::vec4(0 - transform.origin.x, transform.size.y - transform.origin.y, 0, 1);

		drawLine(pos1, pos2, color);
		drawLine(pos2, pos3, color);
		drawLine(pos3, pos4, color);
		drawLine(pos4, pos1, color);
	}




	//render all objects in draw queue and clear queue
	void RendererDebug::submit(Camera2d& cam) {
		//std::sort(drawQueue, drawQueue + draw_it, sort_comparator);

		debugShader->setMatrix4f(cam.getMatrix(), "proj");

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, draw_it * sizeof(LineInstance), drawQueue);
		glBindVertexArray(vao_id);

		debugShader->use();
		glDrawArrays(GL_LINES, 0, draw_it);
		debugShader->unuse();

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		draw_it = 0;
	}

	RendererDebug::~RendererDebug() {
		glDeleteBuffers(1, &vbo_id);
		glDeleteVertexArrays(1, &vao_id);
		std::cout << "RendererDebug Destroyed!\n";
		delete debugShader;
		delete[] drawQueue;
	}
}