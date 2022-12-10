#include "Framebuffer.h"

namespace air {
	Framebuffer::Framebuffer() {

	}

	void Framebuffer::init() {
		glGenFramebuffers(1, &m_fbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);

		glGenTextures(1, &m_texture.id);

		glBindTexture(GL_TEXTURE_2D, m_texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_params.width, m_params.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_params.param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_params.param);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.id, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::cout << "Framebuffer init : tex id : " << m_texture.id << '\n';
	}

	Texture* Framebuffer::getTexture() {
		return &m_texture;
	}



	FramebufferParameters& Framebuffer::getParameters() {
		return m_params;
	}

	Framebuffer::~Framebuffer() {
		std::cout << "Framebuffer destructed, texture id: " << m_texture.id<< " \n";
		glDeleteTextures(1, &m_texture.id);
		glDeleteFramebuffers(1, &m_fbo_id);
	}


	C_RenderTexture::C_RenderTexture() {
		m_framebuffer = new Framebuffer();
		m_framebuffer->init();
	}

	C_RenderTexture::C_RenderTexture(FramebufferParameters _params) {
		m_framebuffer = new Framebuffer();
		m_framebuffer->getParameters() = _params;
		m_framebuffer->init();
	}

	C_RenderTexture::C_RenderTexture(const C_RenderTexture& rt) {
		//std::cout << "copied! mutherfucker!";
		m_framebuffer = rt.m_framebuffer;
		m_camera = rt.m_camera;
	}

	C_RenderTexture& C_RenderTexture::operator=(const C_RenderTexture& rt) {
		//std::cout << "copied assignment! mutherfucker!";
		m_framebuffer = rt.m_framebuffer;
		m_camera = rt.m_camera;

		return *this;
	}

	C_RenderTexture::C_RenderTexture(C_RenderTexture&& rt) {
		//std::cout << "move ! mutherfucker!";
		m_framebuffer = rt.m_framebuffer;
		m_camera = rt.m_camera;

		rt.m_camera = nullptr;
		rt.m_framebuffer = nullptr;
	}

	C_RenderTexture& C_RenderTexture::operator=(C_RenderTexture&& rt) {
		//std::cout << "move assignment! mutherfucker!";
		m_framebuffer = rt.m_framebuffer;
		m_camera = rt.m_camera;

		rt.m_camera = nullptr;
		rt.m_framebuffer = nullptr;

		return *this;
	}


	Texture* C_RenderTexture::getTexture() {
		return m_framebuffer->getTexture();
	}


	Camera2d* C_RenderTexture::getCamera() {
		return m_camera;
	}

	Framebuffer& C_RenderTexture::getFramebuffer() {
		return *m_framebuffer;
	}

	void C_RenderTexture::setCamera(Camera2d& _camera) {
		m_camera = &_camera;
	}

	C_RenderTexture::~C_RenderTexture() {
		delete m_framebuffer;
		std::cout << "C_RenderTexture destructed, texture id: "  << " \n";
	}
}