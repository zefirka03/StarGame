#pragma once
#include "debug.h"
#include "../glIncl.h"

#include "../ecs/air_ecs.h"
#include "Input.h"
#include "../render/air_render.h"
#include "../scripts/air_scripts.h"
#include "../systems/System_Physics.h"

namespace air {
	class Game {
	private:
		static Game* instance;

		GLFWwindow* window;

		Scene* current_scene = nullptr;
		Scene* next_scene = nullptr;

		const char* title;
		int w, h;
		TimeStep ts;
	public:
		Game(const char* _title, int _w, int _h);
		void run(Scene* _scn);

		void goToScene(Scene* _scn);

		static Game& getInstance();
		GLFWwindow* getNativeWindow();

		~Game();
	private:
		void init_imgui();
		void _updateCurrentScene();
	};
}