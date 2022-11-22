#pragma once

#include "air.h"
#include "Scripts/S_PlayerController.h"
#include "Scripts/S_World.h"

using namespace air;

class Scene_try1 : public Scene {
	void onStart() override {
		physics = getSystems().Air_Physics;

		Entity ent_camera = createEntity();
		auto& camera = ent_camera.addComponent<C_Camera2d>(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		ent_camera.addScript<S_Camera2dController>();

		camera.camera.main = true;

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");
		TM.loadTexture("img/empty.png", "empty");

		Entity world_generation = createEntity();
		S_WorldGeneration* WG = world_generation.addScript<S_WorldGeneration>(120, 100);
		
		Entity player = createEntity();
		S_PlayerController* playerController = player.addScript<S_PlayerController>();
		playerController->setWorld(WG);
		playerController->setCamera(&camera);
	}
	void imGui() override {
		ImGui::Checkbox("Physics debug", &physics->debug);
	};
	void onEnd() override {};
private:
	_System_Physics* physics;
};




