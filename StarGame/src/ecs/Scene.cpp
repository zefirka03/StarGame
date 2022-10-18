#include "Scene.h"
#include "Entity.h"
#include "../Components.h"
#include "../core/debug.h"

namespace air {
	Scene::Scene() {
		render = new Renderer2d(300000);
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
	}

	void Scene::onUpdate(float _deltaTime, Camera2d& _cam) {
		//Update scripts
		{
			//Timer t1("Update scripts");
			reg.view<Entity::_C_NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				for (auto script : nsc.Instances) {
					script->OnUpdate(_deltaTime);
				}
			});
		}

		//Render system
		{

			//Timer t1("Render system");
			auto render_view = reg.view<C_Sprite>();
			{
				//Timer t2("Draw");
				render_view.each([&](auto& sprite) {
					render->draw({ sprite.position, sprite.size, sprite.color });
				});
			}
			{
				//Timer t2("Submit");
				render->submit(_cam);
			}
		}
	}

	Scene::~Scene() {
		delete render;
	}
}