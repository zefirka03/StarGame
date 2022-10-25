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

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glfwSwapInterval(1);
	}

	Game& Game::getInstance() {
		return *instance;
	}

	GLFWwindow* Game::getNativeWindow() {
		return window;
	}

	//class S_Controller : public Script {
	//public:
	//	void OnUpdate(float deltaTime) {
	//		if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
	//			t += deltaTime * 5;
	//			getComponent<C_Sprite>().position += glm::vec2(sin(t) * 500 * deltaTime, 0);
	//		}
	//	}
	//
	//	void OnCreate()  {
	//		getComponent<C_Sprite>().position = glm::vec2(300);
	//	}
	//private:
	//	float t = 0;
	//};

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

	//class JustSystem : public System {
	//	void init() override {
	//
	//	}
	//	void update(float _deltaTime) override {
	//		if (Input::isMousePressed_Left()) {
	//			auto ent = scene->createEntity();
	//			auto& tr = ent.getComponent<Transform2d>();
	//			tr.position = glm::vec3(Input::getCursorPos(), 0);
	//			tr.size = glm::vec2(5, 5);
	//			ent.addComponent<C_BoundingBox>() = { tr };
	//		}
	//
	//	}
	//	void terminate() override {
	//
	//	}
	//};

	class S_pos : public Script {
	public:

		void OnUpdate(float deltaTime) {
			getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(), 0);

			C_BoundingBox& bbox = getComponent<C_BoundingBox>();
			bbox.transform.position = glm::vec3(Input::getCursorPos(), 0);
			//std::cout << bbox.colliders.size() << '\n';
			for (int i = 0; i < bbox.colliders.size(); i++) {
				bbox.colliders[i]->_gameObject->getComponent<C_Sprite>().color = glm::vec4(0.5);
				 //bbox.colliders[i]->_gameObject->getComponent<C_Transform2d>().transform.position = glm::vec3(0);
			}
		}

		void OnCreate() {
			getComponent<C_BoundingBox>().check_collisions = true;
			this->getComponent<C_Sprite>().color = glm::vec4(1);
		}
	private:
		bool collided = false;
	};


	void Game::run() {
		Scene scn;
		PhysicsSystem* physics = scn.addSystem<PhysicsSystem>();
		//scn.addSystem<JustSystem>();


		Entity ent_camera = scn.createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(1280.f, 720.f);
		//ent_camera.addScript<S_Camera2dController>();

		TextureManager& TM = scn.getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");

		Entity ent1 = scn.createEntity();
		ent1.addScript<S_pos>();
		//
		auto& sp1 = ent1.addComponent<C_Sprite>(); 
		auto& tr1 = ent1.getComponent<C_Transform2d>(); 
		auto& bbx1 = ent1.addComponent<C_BoundingBox>(); 
		
		//sp1 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3"));
		tr1.entity()->getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("4"));
		tr1.transform = Transform2d( glm::vec3(0,  0, 0), glm::vec2(100, 100) );
		bbx1.transform = Transform2d( glm::vec3(0, 0, 0), glm::vec2(100, 100) );

		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 50; ++j) {
				Entity ent2 = scn.createEntity();
				auto& sp2 = ent2.addComponent<C_Sprite>();
				auto& tr2 = ent2.getComponent<C_Transform2d>().transform = Transform2d( glm::vec3(i*12, j * 12, 0), glm::vec2(10, 10) );
				auto& bbx2 = ent2.addComponent<C_BoundingBox>().transform = Transform2d( glm::vec3(i * 12, j * 12, 0), glm::vec2(10, 10) );
				sp2 = C_Sprite( glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3") );
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

			scn.onUpdate(float(deltaTime));


			//auto bxs = physics->quadTree->retrieve(&ent1.getComponent<C_BoundingBox>());
			//if (bxs.size() > 0)
			//ent1.getComponent<C_Sprite>().color = glm::vec4(0);
			//else ent1.getComponent<C_Sprite>().color = glm::vec4(1);

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