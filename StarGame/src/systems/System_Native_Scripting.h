#pragma once

#include "../ecs/System.h"
#include "../ecs/Script.h"
#include "../ecs/EntityComponent.h"

#include "../render/Renderer2d.h"
#include "../components/Transform.h"

namespace air {
	

	class _System_Native_Scripting : public System {
	public:
		_System_Native_Scripting() {}
		void init() override {
			reg->view<_C_NativeScriptComponent, _C_New_Script>().each([&](auto entity, auto& nsc, auto& _new) {
				for (auto script : nsc.Instances) {
					if (!script->inited) {
						script->m_entity = { entity, scene };
						script->OnCreate();
						script->inited = true;
					}
				}
				reg->remove<_C_New_Script>(entity);
			});
		}
		void update(float _deltaTime) override {
			//process new scriptable objects
			reg->view<_C_NativeScriptComponent, _C_New_Script>().each([&](auto entity, auto& nsc, auto& _new) {
				for (auto script : nsc.Instances) {
					if (!script->inited) {
						script->m_entity = { entity, scene };
						script->OnCreate();
						script->inited = true;
					}
				}
				reg->remove<_C_New_Script>(entity);
			});
			//Update scripts
			
			//Timer t1("Update scripts");
			reg->view<_C_NativeScriptComponent>().each([&](auto& nsc) {
				for (auto script : nsc.Instances) {
					script->OnUpdate(_deltaTime);
				}
			});

			reg->view<_C_Destroyed>().each([&](const auto entity, auto& dst) {
				reg->destroy(entity);
				});
			
		}
		void terminate() override {}
		~_System_Native_Scripting() {}
	private:
	};
}