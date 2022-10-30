#pragma once
#include "EntityComponent.h"

namespace air {
	class Script {
		friend class Scene;
	public:
		template<class T>
		T& getComponent() {
			return m_entity.getComponent<T>();
		}
		Scene* getScene() {
			return m_entity.scene;
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(float _deltaTime) {}
		virtual void OnDestroy() {}
	private:
		Entity m_entity;
	};
}