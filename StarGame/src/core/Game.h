#pragma once
#include "debug.h"
#include "../glIncl.h"

#include "../ecs/air_ecs.h"
#include "../render/air_render.h"

#include "../Components.h"

namespace air {
	class Game {
	public:
		Game(const char* _title, int _w, int _h);
		void run();

		static Game& getInstance();
		GLFWwindow* getNativeWindow();

		~Game();
	private:
		static Game* instance;

		GLFWwindow* window;

		void init_imgui();

		const char* title;
		int w, h;
	};
}