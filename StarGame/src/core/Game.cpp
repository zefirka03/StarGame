#include "Game.h"
#include "Input.h"
#include "debug.h"

namespace air {
	Game* Game::instance = nullptr;

	Game::Game(const char* _title, int _w, int _h) {
		instance = this;

		window = nullptr;
		title = _title;
		w = _w, h = _h;

		if (glfwInit()) {
			window = glfwCreateWindow(w, h, _title, NULL, NULL);
			glfwMakeContextCurrent(window);
		}
		else WA("glfw not inited!");

		if (glewInit())
			WA("glew not inited!");

		init_imgui();

		glfwSwapInterval(1);
	}

	Game& Game::getInstance() {
		return *instance;
	}

	GLFWwindow* Game::getNativeWindow() {
		return window;
	}

	class S_Controller : public Script {
	public:
		void OnUpdate(float deltaTime) {
			if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
				t += deltaTime * 5;
				getComponent<C_Sprite>().position += glm::vec2(sin(t) * 500 * deltaTime, 0);
			}
		}

		void OnCreate()  {
			getComponent<C_Sprite>().position = glm::vec2(300);
		}
	private:
		float t = 0;
	};

	class S_color : public Script {
	public:
		void OnUpdate(float deltaTime) {
			t += deltaTime * 5;
			getComponent<C_Sprite>().color = glm::vec4(abs(sin(t)), abs(sin(t+0.2)), abs(sin(t + 0.42)),1);
		}

		void OnCreate() {
		}
	private:
		float t = 0;
	};


	void Game::run() {
		Scene scn;
		Entity ent_camera = scn.createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(1280.f, 720.f);
		ent_camera.addScript<S_Camera2dController>();

		//Entity ent1 = scn.createEntity();
		//Entity ent2 = scn.createEntity();
		//
		//auto& sp1 = ent1.addComponent<C_Sprite>();
		//auto& sp2 = ent2.addComponent<C_Sprite>();
		//sp1 = { glm::vec2(100), glm::vec2(100), glm::vec4(1, 0, 0, 1)};
		//sp2 = { glm::vec2(0), glm::vec2(100), glm::vec4(0, 1, 0, 1) };
		//ent1.addScript<S_Controller>();

		for (int i = 0; i < 360; ++i) {
			for (int j = 0; j < 640; ++j) {
				auto ent1 = scn.createEntity();
				ent1.addComponent<C_Sprite>() = { glm::vec2(j * 2 , i * 2), glm::vec2(1), glm::vec4(1, 0, 0, 1) };
			}
		}
		
		bool swpInt = 1;

		scn.onStart();

		while (!glfwWindowShouldClose(window)) {
			double deltaTime = ts.elapsed_ms() * 0.001f;
			ts.restart();

			glClear(GL_COLOR_BUFFER_BIT);
			Input::pollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			scn.onUpdate(deltaTime);

			ImGui::Begin("debug");
			{
				if (ImGui::Checkbox("swapInterval", &swpInt))
					glfwSwapInterval(swpInt);
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
		}
		glfwTerminate();
	}

	void Game::init_imgui() {
		const char* glsl_version = "#version 430";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	Game::~Game() {

		//imgui shutdown 
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}