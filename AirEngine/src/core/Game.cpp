#include "Game.h"
#include "Input.h"
#include "debug.h"

#include "../ecs/Scene.h"

namespace air {
	Game* Game::instance = nullptr;

	Game::Game(const char* _title, int _w, int _h, bool _fullscreen) {
		instance = this;

		window = nullptr;
		title = _title;
		w = _w, h = _h;

		if (glfwInit()) {
			if(_fullscreen)
				window = glfwCreateWindow(w, h, _title, glfwGetPrimaryMonitor(), NULL);
			else window = glfwCreateWindow(w, h, _title, NULL, NULL);
			glfwMakeContextCurrent(window);
		}
		else WA("glfw not inited!");

		if (glewInit())
			WA("glew not inited!");

		init_imgui();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glfwWindowHint(GLFW_SAMPLES, 8);
		glEnable(GL_MULTISAMPLE);

		glfwSwapInterval(1);
	}

	Game& Game::getInstance() {
		return *instance;
	}

	GLFWwindow* Game::getNativeWindow() {
		return window;
	}

	void Game::goToScene(Scene* scn) {
		next_scene = scn;
	}

	int Game::getHeight() {
		return h;
	}

	int Game::getWidth() {
		return w;
	}

	void Game::_updateCurrentScene() {
		if (next_scene) {
			delete current_scene;
			current_scene = next_scene;

			//inside onStart() func we add systems/scripts/entities and then we init all of this inside _init func
			current_scene->onStart();
			current_scene->_init();

			next_scene = nullptr;
		}
	}
	

	void Game::run(Scene* scn) {
		goToScene(scn);

		bool swpInt = 1;

		glClearColor(0.1, 0.1, 0.1, 1);

		while (!glfwWindowShouldClose(window)) {
			this->_updateCurrentScene();

			double deltaTime = ts.elapsed_ms() * 0.001f;
			ts.restart();

			glClear(GL_COLOR_BUFFER_BIT);
			Input::pollEvents();
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//update current scene
			current_scene->_onUpdate(float(deltaTime));

			ImGui::Begin("debug");
			{
				current_scene->imGui();

				if (ImGui::Checkbox("swapInterval", &swpInt))
					glfwSwapInterval(swpInt);

			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
		}

		glfwTerminate();
		this->_terminate();
	}

	void Game::init_imgui() {
		const char* glsl_version = "#version 430";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void Game::_terminate() {
		delete current_scene;
	}

	Game::~Game() {

		//imgui shutdown 
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}