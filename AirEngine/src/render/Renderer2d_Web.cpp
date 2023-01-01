#include "Renderer2d_Web.h"

namespace air {
	Renderer2d_Web::Renderer2d_Web(air_sprite_id _sprite_count) {
		assert(_sprite_count < MAX_AVALIABLE_SPRITE_COUNT && "You cant to store so many sprites!");
		maxSpriteCount = _sprite_count;

		drawQueue = new quadInstanceWeb[_sprite_count];
		draw_it = 0;
		printf("Renderer2d_Web: %d video bites allocated\n", sizeof(quadInstanceWeb) * maxSpriteCount);

		spriteShader = new Shader();
		const char* shader =
			#include "shaders/_SpriteShaderWeb.shader"
		;
		spriteShader->loadFromString(
			shader,
			AIR_SHADER_VF
		);
		

		glGenBuffers(1, &vbo_id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(
			GL_ARRAY_BUFFER, maxSpriteCount * sizeof(quadInstanceWeb), nullptr, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteInstanceWeb), (GLvoid*)(sizeof(GLfloat) * 0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInstanceWeb), (GLvoid*)(sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteInstanceWeb), (GLvoid*)(sizeof(GLfloat) * 7));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Renderer2d_Web::draw(const SpriteInstance& _vert) {
		float rotation = _vert.transform.rotation;
		glm::mat2 rot_mat = glm::mat2(	cos(rotation), -sin(rotation),
										sin(rotation), cos(rotation));

		glm::vec2 locPos;

		++_vert.tex->spritesCount[_vert.layer];

		quadInstanceWeb qiw;
		locPos = -_vert.transform.origin + glm::vec2(0, 0);
		qiw.verts[0] = { _vert.transform.position + glm::vec3(rot_mat * locPos, 0), _vert.color, glm::vec2(_vert.textureRect[0], _vert.textureRect[1] + _vert.textureRect[3]), _vert.tex, _vert.layer};
		locPos = -_vert.transform.origin + glm::vec2(_vert.transform.size.x, 0);
		qiw.verts[1] = { _vert.transform.position + glm::vec3(rot_mat * locPos, 0), _vert.color, glm::vec2(_vert.textureRect[0] + _vert.textureRect[2], _vert.textureRect[1] + _vert.textureRect[3]), _vert.tex, _vert.layer};
		locPos = -_vert.transform.origin + glm::vec2(0, _vert.transform.size.y);
		qiw.verts[2] = { _vert.transform.position + glm::vec3(rot_mat * locPos, 0), _vert.color, glm::vec2(_vert.textureRect[0], _vert.textureRect[1]), _vert.tex, _vert.layer};

		locPos = -_vert.transform.origin + glm::vec2(_vert.transform.size.x, 0);
		qiw.verts[3] = { _vert.transform.position + glm::vec3(rot_mat * locPos, 0), _vert.color, glm::vec2(_vert.textureRect[0] + _vert.textureRect[2], _vert.textureRect[1] + _vert.textureRect[3]), _vert.tex, _vert.layer};
		locPos = -_vert.transform.origin + glm::vec2(0, _vert.transform.size.y);
		qiw.verts[4] = { _vert.transform.position + glm::vec3(rot_mat * locPos, 0), _vert.color, glm::vec2(_vert.textureRect[0], _vert.textureRect[1]), _vert.tex, _vert.layer};
		locPos = -_vert.transform.origin + glm::vec2(_vert.transform.size.x, _vert.transform.size.y);
		qiw.verts[5] = { _vert.transform.position + glm::vec3(rot_mat * locPos, 0), _vert.color, glm::vec2(_vert.textureRect[0] + _vert.textureRect[2], _vert.textureRect[1]), _vert.tex, _vert.layer};

		drawQueue[draw_it++] = qiw;
	}

	void Renderer2d_Web::submit(Camera2d& cam, C_RenderTexture* rendTex) {
		stats.last_draw_count = draw_it;

		std::sort(drawQueue, drawQueue + draw_it, texture_sort_comparator);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, draw_it * sizeof(quadInstanceWeb), drawQueue);
		glBindVertexArray(vao_id);

		Camera2d& tmp_cam = cam;
		uint32_t tmp_layer_mask = cam.m_layersBitmask;

		if (rendTex != nullptr) {
			tmp_cam = *rendTex->getCamera();
			glBindFramebuffer(GL_FRAMEBUFFER, rendTex->getFramebuffer().getId());
			glViewport(0, 0, rendTex->getFramebuffer().m_params.width, rendTex->getFramebuffer().m_params.height);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, Game::getInstance().getWidth(), Game::getInstance().getHeight());
		}

		spriteShader->setMatrix4f(tmp_cam.getMatrix(), "proj");

		glClearColor(cam.backgroundColor.x, cam.backgroundColor.y, cam.backgroundColor.z, 1);

		size_t j = 0;
		while (j < draw_it) {
			//jump to another texture
			auto it = (drawQueue + j);
			//ADD FRAMEBUFFER THERE!
			if ((tmp_layer_mask >> it->verts[0].layer) & 1u) {
				glActiveTexture(GL_TEXTURE0);
				if (it->verts[0].tex != nullptr)
					glBindTexture(GL_TEXTURE_2D, it->verts[0].tex->id);

				spriteShader->use();

				glDrawArrays(GL_TRIANGLES, j * 6, it->verts[0].tex->spritesCount[it->verts[0].layer] * 6);
				spriteShader->unuse();
			}

			j += it->verts[0].tex->spritesCount[it->verts[0].layer];
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Renderer2d_Web::clear() {
		int j = 0;
		while (j < draw_it) {
			//jump to another texture
			auto it = (drawQueue + j);

			j += it->verts[0].tex->spritesCount[it->verts[0].layer];
			it->verts[0].tex->spritesCount[it->verts[0].layer] = 0;
		}
		draw_it = 0;
	}

	Renderer2d_Web::~Renderer2d_Web() {
		printf("Renderer2d_Web Destroyed!\n");

		delete[] drawQueue;
		delete spriteShader;
	}
}