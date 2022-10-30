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

		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);

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

	int obj_count = 0;

	class S_color : public Script {
	public:
		void OnUpdate(float deltaTime) {
			t += deltaTime * 5;
			auto& it = getComponent<C_Sprite>().color;
			it = glm::vec4(1, 1, 1, (std::min(1.f, it.a + 0.25f * deltaTime)));
		}

		void OnCreate() {
		}
	private:
		float t = 0;
	};

	class S_pos1 : public Script {
	public:
		void OnUpdate(float deltaTime) {
			t += deltaTime * 5;
			auto& it = getComponent<C_Transform2d>().transform;
			auto& itb = getComponent<C_BoundingBox>().getTransform();
			it.position.x = tr + 5*sin(t);
			itb.position.x = tr + 5*sin(t);
		}

		void OnCreate() {
			tr = getComponent<C_Transform2d>().transform.position.x;
		}
	private:
		float t = 0;
		float  tr;
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
		S_pos(Camera2d& _cam) {
			cam = &_cam;
		}
		void OnUpdate(float deltaTime) override {
			getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(*cam), 0);

			C_BoundingBox& bbox = getComponent<C_BoundingBox>();
			bbox.getTransform().position = glm::vec3(Input::getCursorPos(*cam), 0);
			//std::cout<< glm::vec3(Input::getCursorPos(*cam), 0).x<<'\n';

			auto& colls = bbox.getColliders();
			for (int i = 0; i < colls.size(); i++) {
				colls[i]->_gameObject.getComponent<C_Sprite>().color = glm::vec4(0.0);
				 //bbox.colliders[i]->_gameObject->getComponent<C_Transform2d>().transform.position = glm::vec3(0);
			}

			if (Input::isMousePressed_Right()) {
				++obj_count;
				Entity ent2 = getScene()->createEntity();
				ent2.addScript<S_color>();
				ent2.addScript<S_pos1>();
				auto& sp2 = ent2.addComponent<C_Sprite>();
				auto& tr2 = ent2.getComponent<C_Transform2d>().transform = Transform2d(glm::vec3(Input::getCursorPos(*cam), 0), glm::vec2(10, 10));
				auto& bx2 = ent2.addComponent<C_BoundingBox>().getTransform() = Transform2d(glm::vec3(Input::getCursorPos(*cam), 0), glm::vec2(10, 10));
				sp2 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), getScene()->getTextureManager().getTexture("3"));
			}
		}

		void OnCreate() override {
			getComponent<C_BoundingBox>().setCollisionsCollect(true);
			this->getComponent<C_Sprite>().color = glm::vec4(1);
			this->getComponent<C_Transform2d>().transform.origin = this->getComponent<C_Transform2d>().transform.size / glm::vec2{2.f,2.f};
			this->getComponent<C_BoundingBox>().getTransform().origin = this->getComponent<C_BoundingBox>().getTransform().size / glm::vec2{2.f,2.f};
		}
	private:
		bool collided = false;
		Camera2d* cam;
	};


	void Game::run() {
		Scene scn;
		PhysicsSystem* physics = scn.addSystem<PhysicsSystem>();
		//scn.addSystem<JustSystem>();


		Entity ent_camera = scn.createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(1280.f, 720.f);
		ent_camera.addScript<S_Camera2dController>();

		TextureManager& TM = scn.getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");
		
		Entity ent1 = scn.createEntity();
		ent1.addScript<S_pos>(ent_camera.getComponent<C_Camera2d>().camera);
		//
		auto& sp1 = ent1.addComponent<C_Sprite>(); 
		auto& tr1 = ent1.getComponent<C_Transform2d>(); 
		auto& bbx1 = ent1.addComponent<C_BoundingBox>(); 
		
		//sp1 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3"));
		tr1._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("4"));
		tr1.transform = Transform2d( glm::vec3(0,  0, 0), glm::vec2(100, 100) );
		bbx1.getTransform() = Transform2d( glm::vec3(0, 0, 0), glm::vec2(100, 100) );
		
		bool swpInt = 1;

		scn.onStart();

		float t = 0;
		while (!glfwWindowShouldClose(window)) {
			double deltaTime = ts.elapsed_ms() * 0.001f;
			ts.restart();

			t += deltaTime;

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
				if (ImGui::Checkbox("quadTree", &physics->debug)) {

				}

				ImGui::Text(("Objects_count: " + std::to_string(obj_count)).c_str());
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