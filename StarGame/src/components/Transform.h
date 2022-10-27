#pragma once
#include "../glIncl.h"

namespace air {

	struct Transform2d {
		Transform2d() = default;
		Transform2d(glm::vec3 _position, glm::vec2 _size) : position(_position), size(_size) {};
		glm::vec3 position = glm::vec3(0);
		glm::vec2 size = glm::vec2(1);
		glm::vec2 origin = glm::vec2(0);
		glm::vec2 scale = glm::vec2(1);
		float rotation = 0;
	};

}