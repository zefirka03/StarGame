#include "System_Debug.h"

namespace air {
	_System_Debug::_System_Debug(air_sprite_id sprite_count) {
		render = new RendererDebug(sprite_count);
	}

	void _System_Debug::init() {}

	void _System_Debug::update(float _deltaTime) {

		C_Camera2d* main_camera = nullptr;

		reg->view<C_Camera2d>().each([&](auto& cam) {
			if (cam.camera.main)
				main_camera = &cam;
			});

		auto reg_render = reg->view<C_Sprite, C_Transform2d>(entt::exclude<_C_New>);

		render->submit(main_camera->camera);
		lines.clear();
	}

	void _System_Debug::terminate() {}

	void _System_Debug::drawLine(glm::vec2 _p1, glm::vec2 _p2, glm::vec4 _color) {
		render->drawLine(	glm::vec3(_p1.x, _p1.y, 0),
							glm::vec3(_p2.x, _p2.y, 0),
							_color	);
	}

	void _System_Debug::drawQuad(glm::vec2 _pos, glm::vec2 _size, glm::vec4 _color, glm::vec2 _origin) {
		Transform2d tmp;
		tmp.position = glm::vec3(_pos.x, _pos.y, 0);
		tmp.size = _size;
		tmp.origin = _origin;
		render->drawQuad(tmp, _color);
	}

	_System_Debug::~_System_Debug() {
		delete render;
	}

}