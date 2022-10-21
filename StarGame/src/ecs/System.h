#pragma once

#include <entt.hpp>
#include "../render/Renderer2d.h"

namespace air {
	class Scene;

	class System {
	public:
		virtual void init(entt::registry& _reg) = 0;
		virtual void update(float _deltaTime) = 0;
	protected:
		entt::registry* reg;
	};


	class _System_Render : public System {
	public:
		_System_Render(air_sprite_id sprite_count) {
			render = new Renderer2d(sprite_count);
		}
		void init(entt::registry& _reg) override{
			reg = &_reg;
		}
		void update(float _deltaTime) override {

			C_Camera2d* main_camera = nullptr;

			reg->view<C_Camera2d>().each([&](auto& cam) {
				main_camera = &cam;
			});
			{
				//Timer t1("Render system");
				auto reg_render = reg->view<C_Sprite>();
				{
					//Timer t2("Draw");
					reg_render.each([&](C_Sprite& sprite) {
						
						//auto& transform = reg_render.get<C_Transform2d>(entity);
						render->draw({ sprite.transform, sprite.color, sprite.textureRect, sprite.tex});
					});
				}
				{
					//Timer t2("Submit");
					render->submit(*main_camera);
				}
			}
			//std::cout << '\n';
		}
		~_System_Render() {
			delete render;
		}
	private:
		Renderer2d* render;
	};
}