#pragma once

#include <entt.hpp>
#include "../render/Renderer2d.h"

namespace air {
	class Scene;

	class System {
	public:
		virtual void update(float _deltaTime, entt::registry& reg) = 0;
	};


	class _System_Render : public System {
	public:
		_System_Render(air_sprite_id sprite_count) {
			render = new Renderer2d(sprite_count);
		}
		void update(float _deltaTime, entt::registry& reg) override {

			C_Camera2d* main_camera = nullptr;

			reg.view<C_Camera2d>().each([&](auto& cam) {
				main_camera = &cam;
			});
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
				render->submit(*main_camera);
			}
		}
		~_System_Render() {
			delete render;
		}
	private:
		Renderer2d* render;
	};
}