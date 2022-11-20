#include "Shader.h"

namespace air {

	Shader::Shader() {
		this->_inited = false;
	}

	Shader::Shader(const char* path, uint8_t usings) {
		this->loadFromFile(path, usings);
	}

	void Shader::use() {
#ifdef AIR_DEBUG
		if (!_inited) { WA("Shader does not inited"); return; }
#endif
		glUseProgram(this->prog_id);
	}

	void Shader::unuse() {
		glUseProgram(0);
	}

	bool Shader::isInited() {
		return _inited;
	}
	GLuint Shader::getId() {
		return this->prog_id;
	}

	void Shader::loadFromFile(const char* path, uint8_t usings) {
		this->prog_id = glCreateProgram();
		std::ifstream of;
		of.open(path);

		if (!of.good()) WA("Shader file is not loaded!");

		std::string curr;
		int type = 0;
		std::string vertex_source = "",
			geometry_source = "",
			fragment_source = "";
		while (std::getline(of, curr)) {
			if (curr == "~~vertex~~") type = 0;
			else if (curr == "~~geometry~~") type = 1;
			else if (curr == "~~fragment~~") type = 2;
			else {
				if (type == 0) vertex_source += curr + "\n";
				else if (type == 1) geometry_source += curr + "\n";
				else if (type == 2) fragment_source += curr + "\n";
			}
		}

		const char* vertex_source_c = vertex_source.c_str(),
			* fragment_source_c = fragment_source.c_str(),
			* geometry_source_c = geometry_source.c_str();

		GLuint	vertex_id = glCreateShader(GL_VERTEX_SHADER),
			geometry_id = glCreateShader(GL_GEOMETRY_SHADER),
			fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertex_id, 1, &vertex_source_c, 0);
		glShaderSource(geometry_id, 1, &geometry_source_c, 0);
		glShaderSource(fragment_id, 1, &fragment_source_c, 0);

		glCompileShader(vertex_id);
		glCompileShader(fragment_id);
		if (usings == AIR_SHADER_VGF)
			glCompileShader(geometry_id);

#ifdef AIR_DEBUG
		std::cout << "Shader::Debugging\n";
		GLchar info[2048];
		glGetShaderInfoLog(vertex_id, 2048, 0, info);
		std::cout << info;
		glGetShaderInfoLog(geometry_id, 2048, 0, info);
		std::cout << info;
		glGetShaderInfoLog(fragment_id, 2048, 0, info);
		std::cout << info;
#endif

		glAttachShader(this->prog_id, vertex_id);
		if (usings == AIR_SHADER_VGF)
			glAttachShader(this->prog_id, geometry_id);
		glAttachShader(this->prog_id, fragment_id);

		glLinkProgram(this->prog_id);

		glDeleteShader(vertex_id);
		glDeleteShader(geometry_id);
		glDeleteShader(fragment_id);

		this->_inited = true;
	}

	void Shader::loadFromString(const char* string, uint8_t usings) {
		this->prog_id = glCreateProgram();
		std::string curr;
		int type = 0;
		std::string vertex_source = "",
			geometry_source = "",
			fragment_source = "";
		std::istringstream sa(string);
		while (std::getline(sa, curr)) {
			if (curr == "~~vertex~~") type = 0;
			else if (curr == "~~geometry~~") type = 1;
			else if (curr == "~~fragment~~") type = 2;
			else {
				if (type == 0) vertex_source += curr + "\n";
				else if (type == 1) geometry_source += curr + "\n";
				else if (type == 2) fragment_source += curr + "\n";
			}
		}

		const char* vertex_source_c = vertex_source.c_str(),
			* fragment_source_c = fragment_source.c_str(),
			* geometry_source_c = geometry_source.c_str();

		GLuint	vertex_id = glCreateShader(GL_VERTEX_SHADER),
			geometry_id = glCreateShader(GL_GEOMETRY_SHADER),
			fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertex_id, 1, &vertex_source_c, 0);
		glShaderSource(geometry_id, 1, &geometry_source_c, 0);
		glShaderSource(fragment_id, 1, &fragment_source_c, 0);

		glCompileShader(vertex_id);
		glCompileShader(fragment_id);
		if (usings == AIR_SHADER_VGF)
			glCompileShader(geometry_id);

#ifdef AIR_DEBUG
		GLchar info[2048];
		glGetShaderInfoLog(vertex_id, 2048, 0, info);
		std::cout << info;
		glGetShaderInfoLog(geometry_id, 2048, 0, info);
		std::cout << info;
		glGetShaderInfoLog(fragment_id, 2048, 0, info);
		std::cout << info;
#endif

		glAttachShader(this->prog_id, vertex_id);
		if (usings == AIR_SHADER_VGF)
			glAttachShader(this->prog_id, geometry_id);
		glAttachShader(this->prog_id, fragment_id);

		glLinkProgram(this->prog_id);

		glDeleteShader(vertex_id);
		glDeleteShader(geometry_id);
		glDeleteShader(fragment_id);

		this->_inited = true;
	}

	GLuint Shader::request_location(const char* path) {
		GLuint loc;
		if (locationsCache.find(path) != locationsCache.end())
			loc = locationsCache[path];
		else loc = locationsCache.insert({ path, glGetUniformLocation(this->prog_id, path) }).second;
		return loc;
	}

	void Shader::setMatrix4f(glm::mat4 val, const char* path) {
		this->use();
		glUniformMatrix4fv(request_location(path), 1, GL_FALSE, glm::value_ptr(val));
		this->unuse();
	}

	void Shader::setFloat(GLfloat val, const char* path) {
		this->use();
		glUniform1f(glGetUniformLocation(request_location(path), path), val);
		this->unuse();
	}

	void Shader::setVector2f(glm::vec2 val, const char* path) {
		this->use();
		glUniform2f(glGetUniformLocation(request_location(path), path), val.x, val.y);
		this->unuse();
	}

	void Shader::setVector4f(glm::vec4 val, const char* path) {
		this->use();
		glUniform4f(glGetUniformLocation(request_location(path), path), val.r, val.g, val.b, val.a);
		this->unuse();
	}

	Shader::~Shader() {
		glDeleteProgram(prog_id);
	}

}