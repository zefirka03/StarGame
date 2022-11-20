#include "System_render.h"

namespace air {

	_System_Render::_System_Render(air_sprite_id sprite_count) {
		render = new Renderer2d(sprite_count);
	}

	void _System_Render::init()  {}

	void _System_Render::update(float _deltaTime)  {

		C_Camera2d* main_camera = nullptr;

		reg->view<C_Camera2d>().each([&](auto& cam) {
			if(cam.camera.main)
				main_camera = &cam;
		});

		auto reg_render = reg->view<C_Sprite, C_Transform2d>(entt::exclude<_C_New>);

		reg_render.each([&](C_Sprite& sprite, C_Transform2d& transform) {
			render->draw({ transform.transform, sprite.color, sprite.textureRect, sprite.tex , sprite.layer });
		});
		
		reg->view<C_RenderTexture>().each([&] (C_RenderTexture& _renderTexture) {
			render->submit(*_renderTexture.getCamera(), &_renderTexture);
		});
		render->submit(main_camera->camera);

		render->clear();
	}

	void _System_Render::terminate() {}
	_System_Render::~_System_Render() {
		delete render;
	}

}