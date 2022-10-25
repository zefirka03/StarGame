#pragma once
#include <entt.hpp>
#include "Scene.h"
#include "Component.h"


namespace air {
	class Script;

	class Entity {
		friend class Scene;
	public:
		Entity() = default;
		Entity(entt::entity _id, Scene* _scene);

		template<class T, class ...Args>
		T& addComponent(Args&&... args) {
			static_assert(std::is_base_of<Component, T>::value, "AIR: You are trying to add Class as Component to an Entity, but it is not inherited from Component!");

			auto& it = scene->reg.emplace<T>(entity_handle, std::forward<Args>(args)...);
			it._gameObject = this;
			return it;
		}

		template<class ...T>
		bool hasComponent() {
			return scene->reg.all_of<T...>(entity_handle);
		}

		template<class T>
		T& getComponent() {
			return scene->reg.get<T>(entity_handle);
		}

		template<class T>
		void removeComponent() {
			scene->reg.remove<T>(entity_handle);
		}

		template<class T>
		void addScript() {
			if (nsc == nullptr) 
				nsc = &(this->addComponent<_C_NativeScriptComponent>());
			nsc->Bind<T>();
		}

		void operator=(entt::entity _entt) {
			entity_handle = _entt;
		}

		~Entity();
	private:
		struct _C_NativeScriptComponent : public Component {
			std::vector<std::shared_ptr<Script>> Instances;

			template<typename T>
			void Bind() {
				Instances.emplace_back(std::make_shared<T>());
			}
		};

		_C_NativeScriptComponent* nsc = nullptr;
		Scene* scene = nullptr;
		entt::entity entity_handle{ 0 };
	};

	class Script {
		friend class Scene;
	public:
		template<class T>
		T& getComponent() {
			return m_entity.getComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(float _deltaTime) {}
		virtual void OnDestroy() {}
	private:
		Entity m_entity;
	};

}