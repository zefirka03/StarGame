#pragma once

#include "../air.h"

using namespace air;

class S_color;
class S_pos;


class Scene1 : public Scene {
	void onStart() override {
		physics_sys = addSystem<PhysicsSystem>();

		air::Entity ent_camera = createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(1280.f, 720.f);
		ent_camera.addScript<S_Camera2dController>();

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");

		Entity ent1 = createEntity();
		ent1.addScript<S_pos>(ent_camera.getComponent<C_Camera2d>().camera);
		//
		auto& sp1 = ent1.addComponent<C_Sprite>();
		auto& tr1 = ent1.getComponent<C_Transform2d>();
		auto& bbx1 = ent1.addComponent<C_AABoundingBox>();

		//sp1 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3"));
		tr1._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("4"));
		tr1.transform = Transform2d(glm::vec3(0, 0, 0), glm::vec2(100, 100));
		bbx1.getTransform() = Transform2d(glm::vec3(0, 0, 0), glm::vec2(100, 100));
	}

	void imGui() override {
		if (ImGui::Checkbox("quadTree", &physics_sys->debug)) {}
	}

	void onEnd() override {

	}
private:
	PhysicsSystem* physics_sys;
};

class S_color : public Script {
public:
	void OnUpdate(float deltaTime) {
		auto& it = getComponent<C_Sprite>().color;
		it = glm::vec4(1, 1, 1, (std::min(1.f, it.a + 0.2f * deltaTime)));
	}

	void OnCreate() {
	}
private:
};

class S_pos1 : public Script {
public:
	void OnUpdate(float deltaTime) {
		t += deltaTime * 5;
		auto& it = getComponent<C_Transform2d>().transform;
		auto& itb = getComponent<C_AABoundingBox>().getTransform();
		it.position.x = tr + 5 * sin(t);
		itb.position.x = tr + 5 * sin(t);
	}

	void OnCreate() {
		tr = getComponent<C_Transform2d>().transform.position.x;
	}
private:
	float t = 0;
	float  tr;
};

class S_pos : public Script {
public:
	S_pos(Camera2d& _cam) {
		cam = &_cam;
	}
	void OnUpdate(float deltaTime) override {
		getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(*cam), 0);

		C_AABoundingBox& bbox = getComponent<C_AABoundingBox>();
		bbox.getTransform().position = glm::vec3(Input::getCursorPos(*cam), 0);
		//std::cout<< glm::vec3(Input::getCursorPos(*cam), 0).x<<'\n';

		auto& colls = bbox.getColliders();
		for (int i = 0; i < colls.size(); i++) {
			if (!Input::isKeyPressed(GLFW_KEY_SPACE))
				colls[i]->_gameObject.getComponent<C_Sprite>().color = glm::vec4(0.0);
			else {
				getScene()->destroyEntity(colls[i]->_gameObject);
			}
			//bbox.colliders[i]->_gameObject->getComponent<C_Transform2d>().transform.position = glm::vec3(0);
		}

		if (Input::isMousePressed_Right()) {

			Entity ent2 = getScene()->createEntity();
			ent2.addScript<S_color>();
			ent2.addScript<S_pos1>();
			auto& sp2 = ent2.addComponent<C_Sprite>();
			auto& tr2 = ent2.getComponent<C_Transform2d>().transform = Transform2d(glm::vec3(Input::getCursorPos(*cam), 0), glm::vec2(10, 10));
			auto& bx2 = ent2.addComponent<C_AABoundingBox>().getTransform() = Transform2d(glm::vec3(Input::getCursorPos(*cam), 0), glm::vec2(10, 10));
			sp2 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), getScene()->getTextureManager().getTexture("3"));
		}

		if (Input::isKeyPressed(GLFW_KEY_ESCAPE))Game::getInstance().goToScene(new Scene1());
	}

	void OnCreate() override {
		getComponent<C_AABoundingBox>().setCollisionsCollect(true);
		this->getComponent<C_Sprite>().color = glm::vec4(1);
		this->getComponent<C_Transform2d>().transform.origin = this->getComponent<C_Transform2d>().transform.size / glm::vec2{ 2.f,2.f };
		this->getComponent<C_AABoundingBox>().getTransform().origin = this->getComponent<C_AABoundingBox>().getTransform().size / glm::vec2{ 2.f,2.f };
	}
private:
	bool collided = false;
	Camera2d* cam;
};
