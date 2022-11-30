#include "S_Camera_Player.h"

S_Camera_Player::S_Camera_Player(S_PlayerController* _player) {
	pl_controller = _player;
}


void S_Camera_Player::OnCreate() {
	camera = &getComponent<C_Camera2d>();
	camera->camera.transform.origin = glm::vec2(camera->camera.width / 2.f, camera->camera.height / 2.f);
}

void S_Camera_Player::OnUpdate(float _deltaTime) {
	camera->camera.transform.position = pl_controller->getComponent<C_Transform2d>().transform.position;
}

void S_Camera_Player::OnDestroy() {

}