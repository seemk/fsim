#include "GL/Shader.hpp"
#include <memory>
#include <iostream>

namespace GL
{
	Shader::Shader(Shader::Type type, const std::string& shaderSource)
	{
		shader = glCreateShader(type);
		const char* source = shaderSource.c_str();
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint compileStatus = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus)
		{
			std::cerr << getInfo() << "\n";
			throw std::runtime_error("Shader compilation failure.");
		}
	}

	std::string Shader::getInfo() const
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLogBuf = std::unique_ptr <char[]>(new char[infoLogLength + 1]);
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLogBuf.get());
		return std::string(infoLogBuf.get(), infoLogLength);
	}

	GLuint Shader::getID() const
	{
		return shader;
	}

	std::shared_ptr<Shader> Shader::create(Type type, const std::string& shaderSource)
	{
		return std::make_shared<Shader>(type, shaderSource);
	}

	Shader::~Shader()
	{
		glDeleteShader(shader);
	}
}