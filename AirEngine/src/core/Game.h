#pragma once
#include "debug.h"
#include "../glIncl.h"


namespace air {
	class Scene;

	class Game {
	private:
		static Game* instance;

		GLFWwindow* window;

		Scene* current_scene = nullptr;
		Scene* next_scene = nullptr;

		const char* title;
		int w, h;
		TimeStep ts;

		std::function<void()> main_loop = nullptr;
		void _loop();
	public:
		Game(const char* _title, int _w, int _h, bool _fullscreen = false);
		void run(Scene* _scn);

		void goToScene(Scene* _scn);
		void setViewport(int x, int y);

		static Game& getInstance();
		GLFWwindow* getNativeWindow();
		int getWidth();
		int getHeight();

		double getDeltaTime();

		~Game();
	private:
		void init_imgui();
		void _updateCurrentScene();
		void _terminate();

		double deltaTime = 1.f;
	};
}