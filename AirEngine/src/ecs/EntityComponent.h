#pragma once
#include <vector>
#include <unordered_map>
#include <cassert>
#include "../glIncl.h"

#include "../components/Camera2d.h"
#include "../components/Transform.h"
#include "../render/Texture.h"

namespace air {
	struct _C_NativeScriptComponent;
	class Scene;

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
		void removeComponent();

		template<class T, class ...Args>
		T* addScript(Args&&... args);

		void operator=(entt::entity _entt) {
			entity_handle = _entt;
		}
		void operator=(Entity _entt) {
			entity_handle = _entt.entity_handle;
			scene = _entt.scene;
		}

		~Entity();
		entt::entity entity_handle;
	private:
		_C_NativeScriptComponent* nsc = nullptr;
		Scene* scene = nullptr;
	};

	class Script;

	class Component {
		friend class Entity;
	public:
		Entity _gameObject;
	};

	
	struct C_Sprite : public Component {
		C_Sprite() = default;
		C_Sprite(glm::vec4 _color) : color(_color) {};
		C_Sprite(glm::vec4 _color, glm::vec4 _texRect, Texture* _tex) : color(_color), textureRect(_texRect), tex(_tex) {};
		glm::vec4 color = glm::vec4(1,1,1,1);
		glm::vec4 textureRect = glm::vec4(0,0,1,1);

		Texture* tex = nullptr;
		uint8_t layer = 0;
	};

	struct C_Camera2d : public Component {
		C_Camera2d() = default;
		C_Camera2d(float _w, float _h) : camera(_w, _h) {};
		Camera2d camera;
	};

	struct C_Tag : public Component {
		std::string tag = "Default";
	};


	struct C_Transform2d : public Component {
		Transform2d transform;
	};


	class _C_New : public Component {};
	class _C_Destroyed : public Component {};
}