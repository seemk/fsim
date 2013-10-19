#include "GL\Program.hpp"
#include <GL\glew.h>
#include <GL\Shader.hpp>
#include <memory>
#include <iostream>

namespace GL
{

	Program::Program()
		: program(0)
	{

	}

	Program::Program(std::initializer_list<std::shared_ptr<GL::Shader>> shaders)
		: program(glCreateProgram())
	{
		for (const auto& shader : shaders)
		{
			glAttachShader(program, shader->getID());
		}
		glLinkProgram(program);
		for (const auto& shader : shaders)
		{
			glDetachShader(program, shader->getID());
		}
		GLint linkStatus = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus)
		{
			std::cerr << getInfo() << "\n";
			throw std::runtime_error("Failed to link GL program");
		}

	}

	Program::Program(const std::vector<Shader>& shaders)
		: program(glCreateProgram())
	{
		for (const auto& shader : shaders)
		{
			glAttachShader(program, shader.getID());
		}

		glLinkProgram(program);

		for (const auto& shader : shaders)
		{
			glDetachShader(program, shader.getID());
		}

		GLint linkStatus = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus)
		{
			std::cerr << getInfo() << "\n";
			throw std::runtime_error("Failed to link GL program");
		}
	}

	void Program::destroy()
	{
		glDeleteProgram(program);
	}

	std::string Program::getInfo() const
	{
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLogBuf = std::make_unique<char[]>(infoLogLength + 1);
		glGetProgramInfoLog(program, infoLogLength, nullptr, infoLogBuf.get());
		return std::string(infoLogBuf.get(), infoLogLength);
	}

	GLuint Program::getID() const
	{
		return program;
	}

	void Program::use() const
	{
		glUseProgram(program);
	}

	bool Program::initialized() const
	{
		return program != 0;
	}

	std::shared_ptr<Program> Program::create(const std::vector<Shader>& shaders)
	{
		return std::make_shared<Program>(shaders);
	}

	void Program::setMVPUniform(GLuint location)
	{
		mvpUniformLocation = location;
	}

	GLuint Program::getMVPUniform() const
	{
		return mvpUniformLocation;
	}
}