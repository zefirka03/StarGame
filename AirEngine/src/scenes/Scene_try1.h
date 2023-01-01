#pragma once

#include "../air.h"

using namespace air;

class S_WorldGeneration;
class S_PlayerController;

class Scene_try1 : public Scene {
	void onStart() override {
		physics = getSystems().Air_Physics;

		Entity ent_camera = createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		ent_camera.addScript<S_Camera2dController>();

		camera.camera.main = true;

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");
		TM.loadTexture("img/empty.png", "empty");

		Entity world_generation = createEntity();
		S_WorldGeneration* WG = world_generation.addScript<S_WorldGeneration>(100, 100);
		
		Entity player = createEntity();
		player.addScript<S_PlayerController>();
	}
	void imGui() override {
		ImGui::Checkbox("Physics debug", &physics->debug);
	};
	void onEnd() override {};
private:
	_System_Physics* physics;
};

class S_WorldGeneration : public Script {
public:
	S_WorldGeneration(int _w, int _h) {
		w = _w;
		h = _h;
	}
	void MakeBlock(int _x, int _y) {
		Entity ent = getScene()->createEntity();
		auto& sp = ent.addComponent<C_Sprite>();
		auto& rg = ent.addComponent<C_RigidBody>();
		auto& cl = ent.addComponent<C_Collider_Box2d>();
		auto& tr = ent.getComponent<C_Transform2d>();
		
		auto& TM = getScene()->getTextureManager();

		sp = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("3"));
		tr.transform = Transform2d(glm::vec3(_x * 10, _y * 10, 0), glm::vec2(10, 10), 0, glm::vec2(5, 5));
		rg.Type = C_RigidBody::type::Static;
		cl.size = tr.transform.size;
	}
	

	void OnCreate() override {
		world = new bool* [w];
		for (int x = 0; x < w; ++x) {
			world[x] = new bool[h];
			for (int y = 0; y < h; ++y) {
				world[x][y] = rand() % 2;
				if (world[x][y]) {
					MakeBlock(x, y);
				}
			}
		}
	}
	void OnUpdate(float _deltaTime)override {
		
	}
	void OnDestroy()override {
		for (int x = 0; x < w; ++x)
			delete[] world[x];
		delete[] world;
		printf("World Destructed\n");
	}
	bool** getWorld() {
		return world;
	}
	~S_WorldGeneration() {
		printf( "World\n");
	}
private:
	int w = 10, h = 10;
	bool** world = nullptr;
};

class S_PlayerController : public Script {
	virtual void OnCreate() override {
		auto& TM = getScene()->getTextureManager();
		sp = &getEntity().addComponent<C_Sprite>();
		rg = &getEntity().addComponent<C_RigidBody>();
		cl = &getEntity().addComponent<C_Collider_Box2d>();
		tr = &getEntity().getComponent<C_Transform2d>();

		*sp = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("5"));
		tr->transform = Transform2d(glm::vec3(250, 520, 1), glm::vec2(10, 10), 0, glm::vec2(5, 5));
		rg->Type = C_RigidBody::type::Dynamic;
		cl->size = tr->transform.size;
	}
	virtual void OnUpdate(float _deltaTime) override {
		if (Input::isKeyPressed(GLFW_KEY_W)) {
			rg->setLinearVelocity({ 0, 10});
		}
		if (Input::isKeyPressed(GLFW_KEY_S)) {
			rg->setLinearVelocity({ 0, -10 });
		}
		if (Input::isKeyPressed(GLFW_KEY_A)) {
			rg->setLinearVelocity({ -10 ,  0 });
		}
		if (Input::isKeyPressed(GLFW_KEY_D)) {
			rg->setLinearVelocity({ 10, 0 });
		}
	}
	virtual void OnDestroy() override {}

private:
	C_Sprite* sp;
	C_Transform2d* tr;
	C_Collider_Box2d* cl;
	C_RigidBody* rg;
};

