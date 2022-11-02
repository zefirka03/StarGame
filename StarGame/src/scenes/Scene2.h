#pragma once

#include "../air.h"

using namespace air;

class player_controller;
class enemy;
class enemy_spawner;

class Scene2 : public Scene {
	void onStart() override {
		physics = addSystem<PhysicsSystem>(glm::vec2{ 0,0 }, glm::vec2{1280,720});

		Entity camera = createEntity();
		camera.addComponent<C_Camera2d>(1280, 720);

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/enemies.png", "enemies");
		TM.loadTexture("img/pack1.png", "player");

		Entity player = createEntity();
		player.addComponent<C_Sprite>();
		player.addComponent<C_AABoundingBox>();
		player.addScript<player_controller>(camera.getComponent<C_Camera2d>().camera);

		Entity enemy_sp = createEntity();
		enemy_sp.addScript<enemy_spawner>();
	}

	void imGui() override {
		if(ImGui::Checkbox("quad", &physics->debug)){}
	}
	void onEnd() override {

	}
private:
	PhysicsSystem* physics;
};


class player_controller : public Script {
public:
	player_controller(Camera2d& _cam) {
		cam = &_cam;
	}
	void OnCreate() override {
		TextureManager& TM = getScene()->getTextureManager();

		sprite = &getComponent<C_Sprite>();
		bbox = &getComponent<C_AABoundingBox>();

		*sprite = C_Sprite(glm::vec4(1), { glm::vec2(0), glm::vec2(12, 14) / glm::vec2(12.f,31.f) }, TM.getTexture("player"));

		getComponent<C_Transform2d>().transform.size = glm::vec2(40, 40);
		getComponent<C_Transform2d>().transform.scale = glm::vec2(-1, 1);
		getComponent<C_Transform2d>().transform.origin = glm::vec2(20, 20);

		getComponent<C_AABoundingBox>().getTransform().size = glm::vec2(20, 20);
		getComponent<C_AABoundingBox>().getTransform().origin = glm::vec2(10, 10);
		getComponent<C_AABoundingBox>().setCollisionsCollect(true);
	}
	void OnUpdate(float _deltaTime) override {
		getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(*cam), 0);
		getComponent<C_Transform2d>().transform.rotation = sin(t) / 4;

		bbox->getTransform().position = getComponent<C_Transform2d>().transform.position;

		t += _deltaTime * 5;

		if (bbox->getColliders().size() > 0) Game::getInstance().goToScene(new Scene2());
	}

private:
	C_Sprite* sprite = nullptr;
	C_AABoundingBox* bbox = nullptr;
	Camera2d* cam = nullptr;

	float t = 0;
};

class enemy : public Script {
public:
	void OnCreate() override {
		rotation_ratio = (rand() % 100) / 200.f;
		speed_ratio = (210 + rand() % 1000) / 7.f;

		TextureManager& TM = getScene()->getTextureManager();

		sprite = &getComponent<C_Sprite>();

		glm::vec4 rects[3] = {
			{ glm::vec2(0, 19) / glm::vec2(22.f, 100.f), glm::vec2(16, 9) / glm::vec2(22.f, 100.f) },
			{ glm::vec2(0, 30) / glm::vec2(22.f, 100.f), glm::vec2(18, 14) / glm::vec2(22.f, 100.f) },
			{ glm::vec2(0, 45) / glm::vec2(22.f, 100.f), glm::vec2(21, 14) / glm::vec2(22.f, 100.f) }
		};

		getComponent<C_Sprite>() = C_Sprite(glm::vec4(1), rects[rand() % 3], TM.getTexture("enemies"));

		getComponent<C_Transform2d>().transform.size = glm::vec2(40, 30);
		getComponent<C_Transform2d>().transform.scale = glm::vec2(1, 1);
		getComponent<C_Transform2d>().transform.origin = glm::vec2(20, 15);

		getComponent<C_AABoundingBox>().getTransform().size = glm::vec2(20, 20);
		getComponent<C_AABoundingBox>().getTransform().origin = glm::vec2(10, 10);
	}
	void OnUpdate(float _deltaTime) override {
		getComponent<C_Transform2d>().transform.rotation += _deltaTime * rotation_ratio;
		getComponent<C_Transform2d>().transform.position.x -= _deltaTime * speed_ratio;

		getComponent<C_AABoundingBox>().getTransform().position = getComponent<C_Transform2d>().transform.position;

		if (getComponent<C_Transform2d>().transform.position.x < 50) getScene()->destroyEntity(getEntity());
	}

private:
	C_Sprite* sprite = nullptr;

	float rotation_ratio = 0;
	float speed_ratio = 0;
};

class enemy_spawner : public Script {
	void OnCreate() override {
		
	}
	void OnUpdate(float _deltaTime) override {
		t += _deltaTime;
		if (t > 0.1) {
			t = 0;
			Entity ent = getScene()->createEntity();
			ent.addComponent<C_Sprite>();
			ent.addComponent<C_AABoundingBox>();
			ent.addScript<enemy>();
			ent.getComponent<C_Transform2d>().transform.position = glm::vec3(glm::vec2(1280, rand()%720), 0);
		}
	}

private:
	float t=0;
};