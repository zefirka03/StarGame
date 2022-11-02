#pragma once
#include <vector>
#include "../glIncl.h"
#include "Scene.h"


#include "../components/Camera2d.h"
#include "../components/Transform.h"

namespace air {
	struct _C_NativeScriptComponent;
	class Entity {
		friend class Scene;
		friend class Script;
	public:
		Entity() = default;
		Entity(entt::entity _id, Scene* _scene);

		template<class T, class ...Args>
		T& addComponent(Args&&... args);

		template<class ...T>
		bool hasComponent();

		template<class T>
		T& getComponent();

		template<class T>
		void removeComponent() {
			scene->reg.remove<T>(entity_handle);
		}

		template<class T, class ...Args>
		void addScript(Args&&... args);

		void operator=(entt::entity _entt) {
			entity_handle = _entt;
		}
		void operator=(Entity _entt) {
			entity_handle = _entt.entity_handle;
			scene = _entt.scene;
		}

		~Entity();
	private:
		_C_NativeScriptComponent* nsc = nullptr;
		Scene* scene = nullptr;
		entt::entity entity_handle{ 0 };
	};


	class Component;
	class Script;

	class Component {
		friend class Entity;
	public:
		Entity _gameObject;
	};

	struct _C_NativeScriptComponent : public Component {
		std::vector<std::shared_ptr<Script>> Instances;

		template<class T, class ...Args>
		void Bind(Args&&... args) {
			Instances.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
		}
	};
	struct C_Sprite : public Component {
		C_Sprite() = default;
		C_Sprite(glm::vec4 _color, glm::vec4 _texRect, Texture* _tex) : color(_color), textureRect(_texRect), tex(_tex) {};
		glm::vec4 color;
		glm::vec4 textureRect;

		Texture* tex;
	};

	struct C_Camera2d : public Component {
		C_Camera2d() = default;
		C_Camera2d(float _w, float _h) : camera(_w, _h) {};
		Camera2d camera;
	};

	struct C_Transform2d : public Component {
		Transform2d transform;
	};


	class _C_New : public Component {};
	class _C_Destroyed : public Component {};
	

	template<class T, class ...Args>
	T& Entity::addComponent(Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "AIR: You are trying to add Class as Component to an Entity, but it is not inherited from Component!");

		auto& it = scene->reg.emplace<T>(entity_handle, std::forward<Args>(args)...);
		it._gameObject = Entity(entity_handle, scene);
		return it;
	}

	template<class ...T>
	bool Entity::hasComponent(){
		return scene->reg.all_of<T...>(entity_handle);
	}

	template<class T>
	T& Entity::getComponent() {
		return scene->reg.get<T>(entity_handle);
	}

	template<class T, class ...Args>
	void Entity::addScript(Args&&... args) {
		if (nsc == nullptr)
			nsc = &(this->addComponent<_C_NativeScriptComponent>());
		nsc->Bind<T>(std::forward<Args>(args)...);
	}

}