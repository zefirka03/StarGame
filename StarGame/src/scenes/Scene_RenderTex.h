#pragma once

#include "../air.h"

using namespace air;

class Scene_RenderTex : public Scene {
	void onStart() override {
		air::Entity ent_camera_rendTex = createEntity();
		air::Entity ent_camera = createEntity();
		auto& camera_rendTex = ent_camera_rendTex.addComponent<C_Camera2d>(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		auto& camera = ent_camera.addComponent<C_Camera2d>(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		ent_camera.addScript<S_Camera2dController>();

		camera_rendTex.camera.main = true;

		TextureManager& TM = getTextureManager();

		TM.loadTexture("img/1.png", "1");
		TM.loadTexture("img/2.png", "2");
		TM.loadTexture("img/3.png", "3");
		TM.loadTexture("img/4.png", "4");
		TM.loadTexture("img/5.png", "5");
		TM.loadTexture("img/empty.png", "empty");

		air::Entity ent_renderTex = createEntity();
		auto& rTex = ent_renderTex.addComponent<C_RenderTexture>(FramebufferParameters(1920/4,1080/4,AIR_TEXTURE_NEAREST));
		rTex.setCamera(camera.camera);
		camera_rendTex.camera.m_layersBitmask = 1;
		camera.camera.m_layersBitmask = 2;

		for (int j = 0; j < 100; j++) {
			for (int i = 0; i < 100; i++) {
				Entity ent = createEntity();
				auto& sp = ent.addComponent<C_Sprite>();
				auto& tr = ent.getComponent<C_Transform2d>();

				tr._gameObject.getComponent<C_Sprite>() = C_Sprite(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("3"));
				sp.layer = uint8_t(1u);
				tr.transform = Transform2d(glm::vec3(i * 20, j * 20, 0), glm::vec2(10, 10), 0, glm::vec2(5, 5));
			}
		}

		air::Entity ent_t = createEntity();
		auto& sp1 = ent_t.addComponent<C_Sprite>();
		sp1.tex = rTex.getTexture();
		sp1.color = glm::vec4(1, 1, 1, 1);
		
		ent_t.getComponent<C_Transform2d>().transform.size = glm::vec2(Game::getInstance().getWidth(), Game::getInstance().getHeight());
		ent_t.getComponent<C_Transform2d>().transform.scale = glm::vec2(1.0, -1.0);
		ent_t.getComponent<C_Transform2d>().transform.position = glm::vec3(0, Game::getInstance().getHeight(), 1);
	}
	void imGui() override {};
	void onEnd() override {};
private:
};
