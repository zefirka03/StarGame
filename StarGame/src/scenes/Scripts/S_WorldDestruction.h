#pragma once
#include "air.h"

#include "S_World.h"

using namespace air;

class S_WorldDestruction : public Script{
public:
	S_WorldDestruction(Camera2d* _cam, S_World* _world) {
		camera = _cam;
		world = _world;
	}
	void OnUpdate(float _deltaTime) override {
		if (Input::isMousePressed_Right()) {
			world->DestroyBlock(int(Input::getCursorPos(*camera).x / 16), int(Input::getCursorPos(*camera).y / 16));
		}
	}

private:
	S_World* world;
	Camera2d* camera;
};

