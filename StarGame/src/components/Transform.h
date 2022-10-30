#pragma once
#include "../glIncl.h"

namespace air {

	struct Transform2d {
		Transform2d() = default;
		Transform2d(glm::vec3 _position, glm::vec2 _size) : position(_position), size(_size) {};
		Transform2d(glm::vec3 _position, glm::vec2 _size, float _rotation) : position(_position), size(_size), rotation(_rotation) {};
		Transform2d(glm::vec3 _position, glm::vec2 _size, float _rotation, glm::vec2 _origin) : position(_position), size(_size), rotation(_rotation), origin(_origin) {};
		glm::vec3 position = glm::vec3(0);
		glm::vec2 size = glm::vec2(1);
		glm::vec2 origin = glm::vec2(0);
		glm::vec2 scale = glm::vec2(1);
		float rotation = 0;
	};

}