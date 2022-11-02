#pragma once
#include <entt.hpp>

#include "../render/Renderer2d.h"

#include "System.h"

namespace air {
	class Entity;
	class Game;

	class Scene {
		friend class Entity;
		friend class Game;
	public:
		Scene();
		Entity createEntity();

		void destroyEntity(Entity ent);

		template<class T, class ...Args>
		T* addSystem(Args&&... args) {
			T* it = new T(std::forward<Args>(args)...);
			it->reg = &reg;
			it->scene = this;
			systems.emplace_back(it);
			return it;
		}

		TextureManager& getTextureManager();

		virtual void onStart() = 0;
		virtual void onEnd() = 0;
		virtual void imGui() {};

		~Scene();

	private:
		void _init();
		void _onUpdate(float _deltaTime);

		TextureManager textureManager;
		std::vector<std::shared_ptr<System>> systems;
		entt::registry reg;
	};



}