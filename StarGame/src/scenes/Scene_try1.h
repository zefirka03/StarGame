#pragma once

#include "air.h"
#include "Scripts/S_PlayerController.h"
#include "Scripts/S_World.h"
#include "Scripts/S_Camera_Player.h"
#include "Scripts/S_WorldDestruction.h"
#include <ctime>

using namespace air;

class Scene_try1 : public Scene {
	void onStart() override {
		srand(time(nullptr));

		physics = getSystems().Air_Physics;
		render = getSystems().Air_Render;

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");
		TM.loadTexture("img/empty.png", "empty");

		Entity player = createEntity();
		S_PlayerController* playerController = player.addScript<S_PlayerController>();

		Entity world = createEntity();
		S_World* WG = world.addScript<S_World>(2048, 2048);
		WG->setPlayer(player);
		
		Entity player_camera_ent = createEntity();
		player_camera = &player_camera_ent.addComponent<C_Camera2d>(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		player_camera_ent.addScript<S_Camera_Player>(playerController);
		player_camera->camera.main = true;

		Entity editor_camera_ent = createEntity();
		editor_camera = &editor_camera_ent.addComponent<C_Camera2d>(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		editor_camera_ent.addScript<S_Camera2dController>();

		world.addScript<S_WorldDestruction>(&player_camera->camera, WG);
	}
	void imGui() override {
		ImGui::Text(("Rendered sprites count: " + std::to_string(render->getStates().sprites)).c_str());
		ImGui::Checkbox("Physics debug", &physics->debug);

		if (ImGui::Checkbox("Editor", &editor)) {
			if (editor) {
				player_camera->camera.main = false;
				editor_camera->camera.main = true;
			}
			else {
				player_camera->camera.main = true;
				editor_camera->camera.main = false;
			}
		}

	};
	void onEnd() override {};
private:
	C_Camera2d* player_camera;
	C_Camera2d* editor_camera;

	bool editor = false;

	_System_Physics* physics;
	_System_Render* render;
};




