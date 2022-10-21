#include "Scene.h"
#include "Entity.h"
#include "../core/debug.h"

namespace air {
	Scene::Scene() {
		this->addSystem<_System_Render>(300000);
	}

	Entity Scene::createEntity() {
		Entity ent(reg.create(), this);
		return ent;
	}

	void Scene::onStart() {
		//onStart for scripts
		{
			reg.view<Entity::_C_NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				for(auto script : nsc.Instances){
					script->m_entity = { entity, this };
					script->OnCreate();
				}
			});
		}

		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->init(reg);
		}
	}

	void Scene::onUpdate(float _deltaTime) {
		//Update scripts
		{
			//Timer t1("Update scripts");
			reg.view<Entity::_C_NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				for (auto script : nsc.Instances) {
					script->OnUpdate(_deltaTime);
				}
			});
		}

		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->update(_deltaTime);
		}
	}

	Scene::~Scene() {
	}
}