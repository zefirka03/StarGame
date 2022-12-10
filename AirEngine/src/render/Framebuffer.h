#pragma once
#include "../glIncl.h"
#include <iostream>

#include "../ecs/EntityComponent.h"
#include "Texture.h"

namespace air {

	struct FramebufferParameters {
	public:
		FramebufferParameters() = default;
		FramebufferParameters(uint16_t _width, uint16_t _height, int _param) :
			width(_width), 
			height(_height), 
			param(_param)
		{};


		///

		uint16_t width	= 1024;
		uint16_t height = 1024;

		// AIR_TEXTURE_LINEAR or AIR_TEXTURE_NEAREST
		int param = AIR_TEXTURE_NEAREST;
	};

	class Framebuffer {
		friend class Renderer2d;
	public:
		Framebuffer();
		FramebufferParameters& getParameters();
		Texture* getTexture();
		void init();
		~Framebuffer();
	private:
		GLuint m_fbo_id;

		Texture m_texture;
		FramebufferParameters m_params;
	};

	struct C_RenderTexture : public Component {
	public:
		C_RenderTexture();
		C_RenderTexture(FramebufferParameters _params);
		C_RenderTexture(const C_RenderTexture& rt);
		C_RenderTexture& operator=(const C_RenderTexture& rt);
		C_RenderTexture(C_RenderTexture&& rt);
		C_RenderTexture& operator=(C_RenderTexture&& rt);

		Texture* getTexture();
		void setCamera(Camera2d& _camera);
		Camera2d* getCamera();
		Framebuffer& getFramebuffer();
		~C_RenderTexture();
	private:
		Framebuffer* m_framebuffer = nullptr;
		Camera2d* m_camera = nullptr;
	};

}