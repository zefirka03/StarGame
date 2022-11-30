#pragma once
#include "../glIncl.h"
#include "Transform.h"

namespace air {
	struct cameraBorder {
		float x1, x2;
		float y1, y2;
	};

	struct Camera2d {
		Camera2d() = default;
		Camera2d(float _w, float _h) : width(_w), height(_h) {
			width = _w;
			height = _h;

			transform.size = glm::vec2(_w, _h);

			projection = glm::ortho(0.f, _w, 0.f, _h);
			

			this->transform.position = glm::vec3(0);
			this->transform.origin = glm::vec2(0, 0);
			this->transform.scale = glm::vec2(1);
		};
		inline glm::mat4 getMatrix() const {
			
			return getProjection() * getView();
		}
		inline glm::mat4 getProjection() const {
			return projection;
		}
		inline glm::mat4 getView() const {
			glm::mat4 view;
			view = glm::translate(glm::mat4(1), glm::vec3(transform.origin, 1));
			view = glm::scale(view, glm::vec3(transform.scale, 1));
			view = glm::translate(view, -transform.position);
			return view;
		}

		cameraBorder getBorders() {
			return { transform.position.x - transform.origin.x / transform.scale.x, transform.position.x - (transform.origin.x - width) / transform.scale.x,
					transform.position.y - transform.origin.y / transform.scale.y, transform.position.y - (transform.origin.x - height) / transform.scale.y };
		}

		void move(glm::vec3 _factor) {
			transform.position += _factor;
		}
		void scale(float _factor) {
			transform.scale *= _factor;
		}
		float width = 100, height = 100;
		Transform2d transform;

		bool main = false;
		glm::mat4 projection = glm::mat4(0);
		uint32_t m_layersBitmask = 4294967295; //max
	};
}