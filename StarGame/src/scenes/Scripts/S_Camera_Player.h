#pragma once
#include "air.h"

#include "S_PlayerController.h"

using namespace air;

class S_Camera_Player : public Script{
public:
	S_Camera_Player(S_PlayerController* _player);

	void OnCreate() override;
	void OnUpdate(float _deltaTime) override;
	void OnDestroy() override;

	~S_Camera_Player() {};
private:
	glm::vec2 lastPlayerPos;

	glm::vec2 speed = glm::vec2(0);

	bool updating = false;
	float t = 0;

	C_Camera2d* camera;
	S_PlayerController* pl_controller;
};

