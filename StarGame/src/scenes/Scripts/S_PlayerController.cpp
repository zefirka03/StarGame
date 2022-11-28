#include "S_PlayerController.h"

void S_PlayerController::setWorld(S_World* _world) {
	World = _world;
}

void S_PlayerController::setCamera(C_Camera2d* _camera) {
	Camera = _camera;
}

void S_PlayerController::OnCreate() {
	auto& TM = getScene()->getTextureManager();
	sp = &getEntity().addComponent<C_Sprite>();
	rg = &getEntity().addComponent<C_RigidBody>();
	cl = &getEntity().addComponent<C_Collider_Box2d>();
	tr = &getEntity().getComponent<C_Transform2d>();

	getEntity().getComponent<C_Tag>().tag = "Player";

	*sp = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 5.f / 17.f, 1), TM.getTexture("2"));;
	tr->transform = Transform2d(glm::vec3(250, 1700, 1), glm::vec2(14, 30), 0, glm::vec2(7.f, 15.f));
	rg->Type = C_RigidBody::type::Dynamic;
	rg->setFixedRotation(true);
	cl->size = tr->transform.size;
	cl->restitutionThreshold = 0;
	cl->restitution = 0;

	ent_feet = getScene()->createEntity();
	ent_feet.addScript<S_PlayerFeet>(this);
}

void S_PlayerController::OnUpdate(float _deltaTime)  {
	if (Input::isKeyPressed(GLFW_KEY_W)) {
		if (onGround) {
			rg->setLinearVelocity({ rg->getLinearVelocity().x, 160 });
		}
	}
	if (Input::isKeyPressed(GLFW_KEY_A)) {
		rg->setLinearVelocity({ -128,  rg->getLinearVelocity().y });
		tr->transform.scale.x = -1;
		t += _deltaTime * 8;
	}
	else if (Input::isKeyPressed(GLFW_KEY_D)) {
		rg->setLinearVelocity({ 128, rg->getLinearVelocity().y });
		tr->transform.scale.x = 1;
		t -= _deltaTime * 8;
	}
	else {
		rg->setLinearVelocity({ 0,  rg->getLinearVelocity().y });
		t = 0;
	}

	if (Input::isMousePressed_Right()) {
		World->DestroyBlock(int(Input::getCursorPos(Camera->camera).x / 16), int(Input::getCursorPos(Camera->camera).y / 16));
		//std::cout << int(Input::getCursorPos(Camera->camera).x / 16) << " " << int(Input::getCursorPos(Camera->camera).y / 16) << '\n';
	}
}
void S_PlayerController::OnDestroy()  {}



S_PlayerFeet::S_PlayerFeet(S_PlayerController* _control) {
	h_control = _control;
}

void S_PlayerFeet::OnCreate()  {
	rg_feet = &getEntity().addComponent<C_RigidBody>();
	cl_feet = &getEntity().addComponent<C_Collider_Box2d>();

	rg_feet->setGravityScale(0);
	rg_feet->isSensor = true;
	cl_feet->size = glm::vec2(h_control->tr->transform.size.x * 0.95, 0.5);
	rg_feet->Type = C_RigidBody::type::Dynamic;
	rg_feet->setFixedRotation(true);
	getEntity().getComponent<C_Transform2d>().transform.origin = { cl_feet->size.x / 2, cl_feet->size.y / 2 };
}

void S_PlayerFeet::OnUpdate(float _deltaTime)  {
	rg_feet->setLinearVelocity(h_control->rg->getLinearVelocity());
	rg_feet->setTransform(glm::vec2(h_control->tr->transform.position.x, h_control->tr->transform.position.y - h_control->tr->transform.origin.y), 0);
}

void S_PlayerFeet::OnCollisionEnter(C_RigidBody& _other)  {
	if (_other._gameObject.getComponent<C_Tag>().tag == "Ground")
		h_control->onGround++;
	//std::cout << "cood";
}

void S_PlayerFeet::OnCollisionEnd(C_RigidBody& _other)  {
	if (_other._gameObject.getComponent<C_Tag>().tag == "Ground")
		h_control->onGround--;
	//std::cout << "end";

}