#pragma once
#include "debug.h"
#include "../glIncl.h"

#include "../ecs/air_ecs.h"
#include "../render/air_render.h"
#include "../scripts/air_scripts.h"

namespace air {
	class Game {
	private:
		static Game* instance;

		GLFWwindow* window;

		const char* title;
		int w, h;
		TimeStep ts;
	public:
		Game(const char* _title, int _w, int _h);
		void run();

		static Game& getInstance();
		GLFWwindow* getNativeWindow();

		~Game();
	private:
		void init_imgui();
	};
}