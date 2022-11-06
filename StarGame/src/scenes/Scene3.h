#pragma once

#include "../air.h"
#include "../systems/System_render.h"
#include "../systems/System_Native_Scripting.h"

using namespace air;

class S_color3;
class S_pos3;
class S_pos13;


class Scene3 : public Scene {
	void onStart() override {
		physics_sys = addSystem<PhysicsSystem>(glm::vec2{ -4000, -4000 }, glm::vec2{8000, 8000});

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
		ent1.addScript<S_pos3>(ent_camera.getComponent<C_Camera2d>().camera);
		//
		auto& sp1 = ent1.addComponent<C_Sprite>();
		auto& tr1 = ent1.getComponent<C_Transform2d>();
		auto& bbx1 = ent1.addComponent<C_PhysicalBody>(new BoundingBox());

		//sp1 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3"));
		tr1._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("4"));
		tr1.transform = Transform2d(glm::vec3(0, 0, 0), glm::vec2(30, 30));
		bbx1.getCollider().getTransform() = Transform2d(glm::vec3(0, 0, 0), glm::vec2(30, 30));
	}

	void imGui() override {
		ImGui::Text(("Colliders: " + std::to_string(physics_sys->stats.colliders_count)).c_str());
		ImGui::Text(("Collision Checks: " + std::to_string(physics_sys->stats.coll_checks)).c_str());
		ImGui::Text(("Collision Checks Total: " + std::to_string(physics_sys->stats.total_coll_checks)).c_str());
		ImGui::Text(("Collision Checks Total in Bad Case: " + std::to_string(physics_sys->stats.colliders_count* physics_sys->stats.colliders_count)).c_str());
		if (ImGui::Checkbox("quadTree", &physics_sys->debug)) {}
	}

	void onEnd() override {

	}
private:
	PhysicsSystem* physics_sys;
};

class S_color3 : public Script {
public:
	void OnUpdate(float deltaTime) {
		auto& it = getComponent<C_Sprite>().color;
		it = glm::vec4(1, 1, 1, (std::min(1.f, it.a + 0.2f * deltaTime)));
	}

	void OnCreate() {
	}
private:
};

class S_pos13 : public Script {
public:
	void OnUpdate(float deltaTime) {
		t += deltaTime * 1;
		auto& it = getComponent<C_Transform2d>().transform;
		auto& itb = getComponent<C_PhysicalBody>().getCollider().getTransform();
		it.rotation = 5 * sin(t);
		itb.rotation = 5 * sin(t);
	}

	void OnCreate() {
	}
private:
	float t = 0;
};

class S_pos3 : public Script {
public:
	S_pos3(Camera2d& _cam) {
		cam = &_cam;
	}
	void OnUpdate(float deltaTime) override {
		getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(*cam), 0);

		C_PhysicalBody& bbox = getComponent<C_PhysicalBody>();
		bbox.getCollider().getTransform().position = glm::vec3(Input::getCursorPos(*cam), 0);
		//std::cout<< glm::vec3(Input::getCursorPos(*cam), 0).x<<'\n';

		auto& colls = bbox.getColliders();

		for (int i = 0; i < colls.size(); ++i) {
			
			if (!Input::isKeyPressed(GLFW_KEY_SPACE))
				colls[i].body->_gameObject.getComponent<C_Sprite>().color = glm::vec4(0.0);
			else {
				getScene()->destroyEntity(colls[i].body->_gameObject);
			}
		}

		if (Input::isMousePressed_Right() && !mouse_pressed) {

			Entity ent2 = getScene()->createEntity();
			ent2.addScript<S_color3>();
			ent2.addScript<S_pos13>();
			auto& sp2 = ent2.addComponent<C_Sprite>();
			int sz = 10 + rand() % 100;
			auto& tr2 = ent2.getComponent<C_Transform2d>().transform = Transform2d(glm::vec3(Input::getCursorPos(*cam), 0), glm::vec2(sz, sz),0,glm::vec2(sz/2.f,sz/2.f));
			auto& bx2 = ent2.addComponent<C_PhysicalBody>(new BoundingBox()).getCollider().getTransform() = Transform2d(glm::vec3(Input::getCursorPos(*cam), 0), glm::vec2(sz, sz), 0, glm::vec2(sz/2.f, sz/2.f));
			sp2 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), getScene()->getTextureManager().getTexture("3"));
			//mouse_pressed = true;
		}

		if (Input::isMouseRelease_Right())
			mouse_pressed = false;
	}

	void OnCreate() override {
		getComponent<C_PhysicalBody>().setCollisionsCollect(true);
		this->getComponent<C_Sprite>().color = glm::vec4(1);
		this->getComponent<C_Transform2d>().transform.origin = this->getComponent<C_Transform2d>().transform.size / glm::vec2{ 2.f,2.f };
		this->getComponent<C_PhysicalBody>().getCollider().getTransform().origin = this->getComponent<C_PhysicalBody>().getCollider().getTransform().size / glm::vec2{ 2.f,2.f };
	}
private:
	bool collided = false;
	bool mouse_pressed = false;
	Camera2d* cam;
};
