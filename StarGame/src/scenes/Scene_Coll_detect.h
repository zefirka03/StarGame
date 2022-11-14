#pragma once
#pragma once

#include "../air.h"

using namespace air;

class s_coll;
class s_bullet;

void cont(C_RigidBody& rb) {
	rb._gameObject.getComponent<C_Sprite>().color = glm::vec4(0, 0, 1, 1);
}
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
		ent1.addScript<s_coll>();

		auto& sp1 = ent1.addComponent<C_Sprite>();
		auto& tr1 = ent1.getComponent<C_Transform2d>();
		auto& rg1 = ent1.addComponent<C_RigidBody>();
		auto& c_box1 = ent1.addComponent<C_Collider_Box2d>();

		tr1._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(0.8, 0.2, 0.2, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
		tr1.transform = Transform2d(glm::vec3(0, -50, 0), glm::vec2(100, 2), 0, glm::vec2(50, 1));
		c_box1.size = tr1.transform.size;
		rg1.Type = C_RigidBody::type::Dynamic;
		rg1.mass = 5;
		rg1.collisionEnter = cont;

		Entity ent2 = createEntity();
		auto& sp2 = ent2.addComponent<C_Sprite>();
		auto& tr2 = ent2.getComponent<C_Transform2d>();
		auto& rg2 = ent2.addComponent<C_RigidBody>();
		auto& c_box2 = ent2.addComponent<C_Collider_Box2d>();
		
		tr2._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(0.8, 0.8, 0.8, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
		tr2.transform = Transform2d(glm::vec3(0, 50, 0), glm::vec2(1000, 2), 0, glm::vec2(500, 1));
		c_box2.size = tr2.transform.size;
		rg2.Type = C_RigidBody::type::Static;

		for (int i = 0; i < 100; i++) {
			Entity ent3 = createEntity();
			auto& sp3 = ent3.addComponent<C_Sprite>();
			auto& tr3 = ent3.getComponent<C_Transform2d>();
			auto& rg3 = ent3.addComponent<C_RigidBody>();
			auto& c_box3 = ent3.addComponent<C_Collider_Box2d>();

			tr3._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(0.8, 0.8, 0.8, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
			tr3.transform = Transform2d(glm::vec3(i * 20, 0, 0), glm::vec2(10, 10), 0, glm::vec2(5, 5));
			c_box3.size = tr3.transform.size;
			rg3.Type = C_RigidBody::type::Dynamic;
		}
	}

	void onEnd() override {

	}
private:
};

class s_bullet : public Script {
public:
	void OnCreate() override {
	}
	void Init(glm::vec3 position) {
		auto ent = getEntity();
		auto& tr = ent.getComponent<C_Transform2d>();
		auto& sp2 = ent.addComponent<C_Sprite>();
		auto& rg  = ent.addComponent<C_RigidBody>();
		auto& col = ent.addComponent<C_Collider_Box2d>();
		rg.Type = C_RigidBody::type::Dynamic;
		sp2 = C_Sprite(glm::vec4(0.8, 0.8, 0.8, 1), glm::vec4(0, 0, 1, 1), getScene()->getTextureManager().getTexture("empty"));
		tr.transform = Transform2d(position, glm::vec2(10));
		col.size = tr.transform.size;
	}

	void OnUpdate(float _deltaTime) override {
		
	}
	void OnDestroy()  override {}
};


class s_coll : public Script {
public:
	void OnCreate() override {
		
	}
	void OnUpdate(float _deltaTime) override {
		if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
			auto ent = getScene()->createEntity();
			auto bull = ent.addScript<s_bullet>();
			bull->Init(getComponent<C_Transform2d>().transform.position);
		}


		if (Input::isKeyPressed(GLFW_KEY_W) && !app) {
			//std::cout << getComponent<C_RigidBody>().mass << '\n';
			getComponent<C_RigidBody>().h_body->ApplyLinearImpulseToCenter({ 0, -1500 * _deltaTime }  , 1);
			//app = true;
		}
		else if (Input::isKeyReleased(GLFW_KEY_W)) {
			//app = false;
		}
		if (Input::isKeyPressed(GLFW_KEY_S)) {
			getComponent<C_RigidBody>().h_body->ApplyLinearImpulseToCenter({ 0, 1500 * _deltaTime }, 1);
		}
		if (Input::isKeyPressed(GLFW_KEY_A)) {
			getComponent<C_RigidBody>().h_body->ApplyLinearImpulseToCenter({ -1500 * _deltaTime,  0}, 1);
		}
		if (Input::isKeyPressed(GLFW_KEY_D)) {
			getComponent<C_RigidBody>().h_body->ApplyLinearImpulseToCenter({ 1500 * _deltaTime, 0 }, 1);
		}
		
	}
	void OnDestroy()  override {}
	float app = false;
};

