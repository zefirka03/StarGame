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
		T* addSystem(Args&&... args) {
			T* it = new T(std::forward<Args>(args)...);
			it->reg = &reg;
			it->scene = this;
			systems.emplace_back(it);
			return it;
		}

		void onStart();
		void onUpdate(float _deltaTime);

		TextureManager& getTextureManager();

		~Scene();

	private:
		TextureManager textureManager;
		std::vector<std::shared_ptr<System>> systems;
		entt::registry reg;
	};



}