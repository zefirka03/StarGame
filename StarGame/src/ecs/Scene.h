#pragma once
#include <entt.hpp>

#include "../render/Renderer2d.h"
#include "../render/Camera2d.h"

namespace air {

	//class System {
	//public:
	//	virtual void update(float _deltaTime) = 0;
	//
	//private:
	//	Scene* scn;
	//};
	//
	//class System_Render : public System {
	//public:
	//	void update(float _deltaTime) override {
	//		scn->reg
	//	}
	//};

	class Entity;

	class Scene {
		friend class System;
		friend class Entity;
	public:
		Scene();
		Entity createEntity();
		void onStart();
		void onUpdate(float _deltaTime, Camera2d& _cam);
		~Scene();
	private:
		entt::registry reg;
		Renderer2d* render;
	};

}