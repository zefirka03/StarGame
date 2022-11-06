#include "Scene.h"
#include "Script.h"
#include "../core/debug.h"
#include "../components/Transform.h"
#include "../systems/System_render.h"
#include "../systems/System_Native_Scripting.h"
#include "../physics/PhysicsQuadTree.h"


namespace air {
	Scene::Scene() {
		this->addSystem<_System_Native_Scripting>();
		this->addSystem<PhysicsSystem>();
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

		//Update destroyed after systems
		reg.view<_C_Destroyed>().each([&](const auto entity, auto& dst) {
			reg.destroy(entity);
		});

	}

	TextureManager& Scene::getTextureManager() {
		return textureManager;
	}

	Scene::~Scene() {
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			(*it)->terminate();
		}
	}
}