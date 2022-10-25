#pragma once
#include "../glIncl.h"
#include "../ecs/Component.h"
#include "../render/TextureManager.h"

namespace air {
	struct C_Sprite : public Component {
		C_Sprite() = default;
		C_Sprite(glm::vec4 _color, glm::vec4 _texRect, Texture* _tex) : color(_color), textureRect(_texRect), tex(_tex) {};
		glm::vec4 color;
		glm::vec4 textureRect;

		Texture* tex;
	};
}