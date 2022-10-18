#pragma once
#include "Game.h"

namespace air {
	class Input {
	public:
		static bool isKeyPressed(int _keycode) {
			return glfwGetKey(Game::getInstance().getNativeWindow(), _keycode);
		}
	};
}