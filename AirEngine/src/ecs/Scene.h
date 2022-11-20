#pragma once

#include "EntityComponent.h"
#include "../render/Renderer2d.h"
#include <unordered_map>
#include "System.h"

namespace air {
	class _System_Physics;

	class Entity;
	class Game;

	class Scene {
		friend class Entity;
		friend class Game;
	private:
		struct systems_handler {
			_System_Physics* Air_Physics;
		} systems_handles;

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

		systems_handler& getSystems() {
			return systems_handles;
		}

		TextureManager& getTextureManager();

		virtual void onStart() = 0;
		virtual void onEnd() = 0;
		virtual void imGui() {};

		~Scene();

	private:
		void _init();
		void _onUpdate(float _deltaTime);
		void _terminate();

		TextureManager textureManager;
		std::vector<std::shared_ptr<System>> systems;
		entt::registry reg;
	};


	template<class T, class ...Args>
	T& Entity::addComponent(Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "AIR: You are trying to add Class as Component to an Entity, but it is not inherited from Component!");

		auto& it = scene->reg.emplace<T>(entity_handle, std::forward<Args>(args)...);
		it._gameObject = Entity(entity_handle, scene);
		return it;
	}

	template<class ...T>
	bool Entity::hasComponent() {
		return scene->reg.all_of<T...>(entity_handle);
	}

	template<class T>
	T& Entity::getComponent() {
		return scene->reg.get<T>(entity_handle);
	}

	template<class T>
	void Entity::removeComponent() {
		scene->reg.remove<T>(entity_handle);
	}
}