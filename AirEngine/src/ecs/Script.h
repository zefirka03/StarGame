#pragma once
#include "EntityComponent.h"

namespace air {
	class _System_Native_Scripting;

	class C_RigidBody;

	class _C_New_Script : public Component {};

	struct _C_NativeScriptComponent : public Component {
		std::vector<std::shared_ptr<Script>> Instances;

		template<class T, class ...Args>
		T* Bind(Args&&... args) {
			Instances.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
			Instances.back().get()->m_entity = _gameObject;
			return (T*)Instances.back().get();
		}
	};

	class Script {
		friend class Scene;
		friend class _System_Native_Scripting;
		friend class Air_B2dContactListener;
		friend struct _C_NativeScriptComponent;
	public:
		template<class T>
		T& getComponent() {
			return m_entity.getComponent<T>();
		}
		Scene* getScene() {
			return m_entity.scene;
		}
		Entity getEntity() {
			return m_entity;
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(float _deltaTime) {}
		virtual void OnDestroy() {}

		virtual void OnCollisionEnter(C_RigidBody& _other) {}
		virtual void OnCollisionEnd(C_RigidBody& _other) {}
	private:
		Entity m_entity;
		bool inited = false;
	};
	
	template<class T, class ...Args>
	T* Entity::addScript(Args&&... args) {
		if (nsc == nullptr)
			nsc = &(this->addComponent<_C_NativeScriptComponent>());
		if(!this->hasComponent<_C_New_Script>())
			this->addComponent<_C_New_Script>();
		return nsc->Bind<T>(std::forward<Args>(args)...);
	}
}