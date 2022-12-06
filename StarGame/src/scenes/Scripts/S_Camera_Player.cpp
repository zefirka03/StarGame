#include "S_Camera_Player.h"

constexpr float PI = glm::pi<float>();

S_Camera_Player::S_Camera_Player(S_PlayerController* _player) {
	pl_controller = _player;
}


void S_Camera_Player::OnCreate() {
	camera = &getComponent<C_Camera2d>();
	camera->camera.transform.origin = glm::vec2(camera->camera.width / 2.f, camera->camera.height / 2.f);
	lastPlayerPos = { pl_controller->getComponent<C_Transform2d>().transform.position.x,
					  pl_controller->getComponent<C_Transform2d>().transform.position.y };
}

float speed_func(float t) {
	return 0.5f * PI * cos(PI * (t - 0.5f));
}

void S_Camera_Player::OnUpdate(float _deltaTime) {
	glm::vec2 curPlayerPos = {	pl_controller->getComponent<C_Transform2d>().transform.position.x,
								pl_controller->getComponent<C_Transform2d>().transform.position.y };
	
	glm::vec2 curCameraPos = { camera->camera.transform.position.x,
							   camera->camera.transform.position.y };


	speed = (curPlayerPos - curCameraPos) * _deltaTime;;

	float speed_val = speed.x;
	float sgn = (speed.x > 0 ? 1.f : -1.f);

	camera->camera.transform.position.x += speed.x;
	camera->camera.transform.position.y += speed.y;

}

void S_Camera_Player::OnDestroy() {

}