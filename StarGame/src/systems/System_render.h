#pragma once

#include "../ecs/System.h"
#include "../ecs/EntityComponent.h"

#include "../render/Renderer2d.h"
#include "../components/Transform.h"

namespace air {
	class _System_Render : public System {
	public:
		_System_Render(air_sprite_id sprite_count) {
			render = new Renderer2d(sprite_count);
		}
		void init() override {
		}
		void update(float _deltaTime) override {

			C_Camera2d* main_camera = nullptr;

			reg->view<C_Camera2d>().each([&](auto& cam) {
				main_camera = &cam;
				});

			auto reg_render = reg->view<C_Sprite, C_Transform2d>(entt::exclude<_C_New>);

			reg_render.each([&](C_Sprite& sprite, C_Transform2d& transform) {

				render->draw({ transform.transform, sprite.color, sprite.textureRect, sprite.tex });
				});

			render->submit(main_camera->camera);
		}
		void terminate() override {}
		~_System_Render() {
			delete render;
		}
	private:
		Renderer2d* render;
	};
}