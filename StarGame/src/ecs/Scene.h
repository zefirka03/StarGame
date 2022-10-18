#pragma once
#include <entt.hpp>

#include "../render/Renderer2d.h"

#include "System.h"

namespace air {
	class Entity;

	class Scene {
		friend class Entity;
	public:
		Scene();
		Entity createEntity();

		template<class T, class ...Args>
		void addSystem(Args&&... args) {
			systems.emplace_back(new T(std::forward<Args>(args)...));
		}

		void onStart();
		void onUpdate(float _deltaTime);

		~Scene();
	private:
		std::vector<std::shared_ptr<System>> systems;
		entt::registry reg;
	};



}