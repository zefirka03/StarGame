#include "Scene.h"
#include "Script.h"
#include "../core/debug.h"
#include "../components/Transform.h"
#include "../systems/System_render.h"
#include "../systems/System_Native_Scripting.h"
#include "../systems/System_Physics.h"


namespace air {
	Scene::Scene() {
		this->addSystem<_System_Native_Scripting>();
		systems_handles.Air_Physics =  this->addSystem<_System_Physics>();
		systems_handles.Air_Render = this->addSystem<_System_Render>(300000+10);
	}

	Entity Scene::createEntity() {
		Entity ent(reg.create(), this);
		ent.addComponent<C_Transform2d>();
		ent.addComponent<C_Tag>();
		ent.addComponent<_C_New>();
		return ent;
	}


	void Scene::destroyEntity(Entity ent) {
		if (!ent.hasComponent<_C_Destroyed>()) {
			ent.addComponent<_C_Destroyed>();
		}
	}

	void Scene::_init() {
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->init();
		}
	}

	void Scene::_onUpdate(float _deltaTime) {
		//process just new objects
		reg.view<_C_New>().each([&](auto entity, auto& n) {
			reg.remove<_C_New>(entity);
		});

		//update systems
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->update(_deltaTime);
		}

		//last update systems
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->updateLast(_deltaTime);
		}

		//Update destroyed after systems
		reg.view<_C_Destroyed>().each([&](const auto entity, auto& dst) {
			reg.destroy(entity);
		});


	}
	bool Scene::isValidEntity(Entity ent) {
		return reg.valid(ent.entity_handle);
	}

	TextureManager& Scene::getTextureManager() {
		return textureManager;
	}

	void Scene::_terminate() {
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->terminate();
		}
		reg.clear();
	}

	Scene::~Scene() {
		this->_terminate();
	}
}