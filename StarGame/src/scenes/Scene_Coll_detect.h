#pragma once
#pragma once

#include "../air.h"

using namespace air;
class S_pos31;

class Scene_Coll_detect : public Scene {
	void onStart() override {


		air::Entity ent_camera = createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(1280.f, 720.f);
		ent_camera.addScript<S_Camera2dController>();

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");
		TM.loadTexture("img/empty.png", "empty");

		Entity ent1 = createEntity();
		ent1.addScript<S_pos31>();

		//
		auto& sp1 = ent1.addComponent<C_Sprite>();
		auto& tr1 = ent1.getComponent<C_Transform2d>();
		auto& bbx1 = ent1.addComponent<C_PhysicalBody>(new BoundingBox());
		bbx1.type = body_type::rigit;
		bbx1.setCollisionsCollect(true);

		Entity ent2 = createEntity();
		//
		auto& sp2 = ent2.addComponent<C_Sprite>();
		auto& tr2 = ent2.getComponent<C_Transform2d>();
		auto& bbx2 = ent2.addComponent<C_PhysicalBody>(new BoundingBox());
		bbx2.type = body_type::solid;

		//sp1 = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0,0,1,1), TM.getTexture("3"));
		tr1._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(0.8, 0.2, 0.2, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
		tr1.transform = Transform2d(glm::vec3(0, -50, 0), glm::vec2(30, 30), glm::pi<float>()/4.f);
		bbx1.getCollider().getTransform() = Transform2d(glm::vec3(0, -50, 0), glm::vec2(30, 30), glm::pi<float>() / 4.f);

		

		tr2._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(0.8, 0.8, 0.8, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
		tr2.transform = Transform2d(glm::vec3(0, 0, 0), glm::vec2(1000, 100));
		bbx2.getCollider().getTransform() = Transform2d(glm::vec3(0, 0, 0), glm::vec2(1000, 100));
	}



	void onEnd() override {

	}
private:
	PhysicsSystem* physics_sys;
};

class S_pos31 : public Script {
public:
	void OnUpdate(float deltaTime) {
		if (Input::isKeyPressed(GLFW_KEY_D)) bbx->properties.velocity.x = 150;
		else if (Input::isKeyPressed(GLFW_KEY_A)) bbx->properties.velocity.x = -150;
		else bbx->properties.velocity.x = 0;
		if (Input::isKeyPressed(GLFW_KEY_W)) bbx->properties.velocity.y = -150;
		
	}

	void OnCreate() {
		bbx = &getComponent<C_PhysicalBody>();
	}
private:
	C_PhysicalBody* bbx;
	float t = 0;
};