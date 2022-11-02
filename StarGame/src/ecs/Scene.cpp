#include "Scene.h"
#include "Script.h"
#include "../core/debug.h"
#include "../components/Transform.h"
#include "../systems/System_render.h"


namespace air {
	Scene::Scene() {
		this->addSystem<_System_Render>(300000+10);
	}

	Entity Scene::createEntity() {
		Entity ent(reg.create(), this);;
		ent.addComponent<C_Transform2d>();
		ent.addComponent<_C_New>();
		return ent;
	}

	void Scene::destroyEntity(Entity ent) {
		if (!ent.hasComponent<_C_Destroyed>()) {
			
			ent.addComponent<_C_Destroyed>();

		}
	}

	void Scene::_init() {
		//onStart for scripts
		{
			reg.view<_C_NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				for(auto script : nsc.Instances){
					script->m_entity = { entity, this };
					script->OnCreate();
				}
			});
		}

		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->init();
		}
	}

	void Scene::_onUpdate(float _deltaTime) {
		//process new scriptable objects
		reg.view<_C_NativeScriptComponent, _C_New>().each([&](auto entity, auto& nsc, auto& n) {
			for (auto script : nsc.Instances) {
				script->m_entity = { entity, this };
				script->OnCreate();
			}
			reg.remove<_C_New>(entity);
		});
		//process just new objects
		reg.view<_C_New>().each([&](auto entity, auto& n) {
			reg.remove<_C_New>(entity);
		});


		//Update scripts
		{
			//Timer t1("Update scripts");
			reg.view<_C_NativeScriptComponent>(entt::exclude<_C_New>).each([&](auto& nsc) {
				for (auto script : nsc.Instances) {
					script->OnUpdate(_deltaTime);
				}
			});
		}


		//Update destroyed after scripts
		reg.view<_C_Destroyed>().each([&](const auto entity, auto& dst) {
			reg.destroy(entity);
		});

		//update systems
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->update(_deltaTime);
		}

		//Update destroyed after systems
		reg.view<_C_Destroyed>().each([&](const auto entity, auto& dst) {
			reg.destroy(entity);
		});

	}

	TextureManager& Scene::getTextureManager() {
		return textureManager;
	}

	Scene::~Scene() {

	}
}