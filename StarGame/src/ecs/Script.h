#pragma once
#include "EntityComponent.h"

namespace air {
	class _System_Native_Scripting;

	class _C_New_Script : public Component {};

	struct _C_NativeScriptComponent : public Component {
		std::vector<std::shared_ptr<Script>> Instances;

		template<class T, class ...Args>
		void Bind(Args&&... args) {
			Instances.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
		}
	};

	class Script {
		friend class Scene;
		friend class _System_Native_Scripting;
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
	private:
		Entity m_entity;
		bool inited = false;
	};
	
	template<class T, class ...Args>
	void Entity::addScript(Args&&... args) {
		if (nsc == nullptr)
			nsc = &(this->addComponent<_C_NativeScriptComponent>());
		nsc->Bind<T>(std::forward<Args>(args)...);
		if(!this->hasComponent<_C_New_Script>())
			this->addComponent<_C_New_Script>();
	}
}