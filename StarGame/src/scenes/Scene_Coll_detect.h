#pragma once
#pragma once

#include "../air.h"

using namespace air;

class S_pos4;
class S_rotate;


class Scene_Coll_detect : public Scene {
	void onStart() override {
		physics_sys = addSystem<PhysicsSystem>(glm::vec2{ -4000, -4000 }, glm::vec2{ 8000, 8000 });

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
		ent1.addScript<S_pos4>(ent_camera.getComponent<C_Camera2d>().camera);
		//
		auto& sp1 = ent1.addComponent<C_Sprite>();
		auto& tr1 = ent1.getComponent<C_Transform2d>();
		auto& bbx1 = ent1.addComponent<C_BoundingBox>();

		Entity ent2 = createEntity();
		ent2.addScript<S_rotate>();
		//
		auto& sp2 = ent2.addComponent<C_Sprite>();
		auto& tr2 = ent2.getComponent<C_Transform2d>();
		auto& bbx2 = ent2.addComponent<C_BoundingBox>();

		//sp1 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3"));
		tr1._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("4"));
		tr1.transform = Transform2d(glm::vec3(0, 0, 0), glm::vec2(30, 30));
		bbx1.getTransform() = Transform2d(glm::vec3(0, 0, 0), glm::vec2(30, 30));

		tr2._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("3"));
		tr2.transform = Transform2d(glm::vec3(0, 0, 0), glm::vec2(100, 100));
		bbx2.getTransform() = Transform2d(glm::vec3(0, 0, 0), glm::vec2(100, 100));
	}

	void imGui() override {
		ImGui::Text(("Colliders: " + std::to_string(physics_sys->stats.colliders_count)).c_str());
		ImGui::Text(("Collision Checks: " + std::to_string(physics_sys->stats.coll_checks)).c_str());
		ImGui::Text(("Collision Checks Total: " + std::to_string(physics_sys->stats.total_coll_checks)).c_str());
		ImGui::Text(("Collision Checks Total in Bad Case: " + std::to_string(physics_sys->stats.colliders_count * physics_sys->stats.colliders_count)).c_str());
		if (ImGui::Checkbox("quadTree", &physics_sys->debug)) {}
	}

	void onEnd() override {

	}
private:
	PhysicsSystem* physics_sys;
};

class S_pos4 : public Script {
public:
	S_pos4(Camera2d& _cam) {
		cam = &_cam;
		deb_rend = new RendererDebug(100);
	}
	void OnUpdate(float deltaTime) override {
		getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(*cam), 0);

		C_BoundingBox& bbox = getComponent<C_BoundingBox>();
		bbox.getTransform().position = glm::vec3(Input::getCursorPos(*cam), 0);

		if (Input::isMousePressed_Right()) {
			if (!mouse_pressed_right) {
				first_point = Input::getCursorPos(*cam);
				mouse_pressed_right = true;
			}
			second_point = Input::getCursorPos(*cam);

			deb_rend->drawLine(glm::vec3(first_point, 0), glm::vec3(second_point, 0), { 0,1,1,1 });
			deb_rend->submit(*cam);
		}

		if (Input::isMouseRelease_Right())
			mouse_pressed_right = false;
	}

	void OnCreate() override {
		getComponent<C_BoundingBox>().setCollisionsCollect(true);
		this->getComponent<C_Sprite>().color = glm::vec4(1);
		this->getComponent<C_Transform2d>().transform.origin = this->getComponent<C_Transform2d>().transform.size / glm::vec2{ 2.f,2.f };
		this->getComponent<C_BoundingBox>().getTransform().origin = this->getComponent<C_BoundingBox>().getTransform().size / glm::vec2{ 2.f,2.f };
	}

	void OnDestroy() override {
		delete deb_rend;
	}
private:
	bool collided = false;
	bool mouse_pressed_right = false;

	glm::vec2 first_point;
	glm::vec2 second_point;

	RendererDebug* deb_rend;

	Camera2d* cam;
};


class S_rotate : public Script {
public:
	S_rotate() {

	}
	void OnUpdate(float deltaTime) override {
		getComponent<C_Transform2d>().transform.rotation += deltaTime * 2;
		getComponent<C_BoundingBox>().getTransform().rotation += deltaTime * 2;
	}

	void OnCreate() override {
		getComponent<C_BoundingBox>().setCollisionsCollect(true);
		this->getComponent<C_Sprite>().color = glm::vec4(1);
		this->getComponent<C_Transform2d>().transform.origin = this->getComponent<C_Transform2d>().transform.size / glm::vec2{ 2.f,2.f };
		this->getComponent<C_BoundingBox>().getTransform().origin = this->getComponent<C_BoundingBox>().getTransform().size / glm::vec2{ 2.f,2.f };
	}


};