#include "GL/Program.hpp"
#include <GL/glew.h>
#include <GL/Shader.hpp>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace GL
{

	Program::Program()
		: program(0)
		, sharedUniformsBlockIndex(GL_INVALID_INDEX)
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

	void Program::setSharedUniformBlockIndex(const std::string& uniformBlockName)
	{
		sharedUniformsBlockIndex = glGetUniformBlockIndex(program, uniformBlockName.c_str());
	}

	GLuint Program::getSharedUniformBlockIndex() const
	{
		return sharedUniformsBlockIndex;
	}

	void Program::destroy()
	{
		glDeleteProgram(program);
	}

	std::string Program::getInfo() const
	{
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLogBuf = std::unique_ptr<char[]>(new char[infoLogLength + 1]);
		glGetProgramInfoLog(program, infoLogLength, nullptr, infoLogBuf.get());
		return std::string(infoLogBuf.get(), infoLogLength);
	}

	GLuint Program::getID() const
	{
		return program;
	}

	bool Program::initialized() const
	{
		return program != 0;
	}

	std::shared_ptr<Program> Program::create(const std::vector<Shader>& shaders)
	{
		return std::make_shared<Program>(shaders);
	}

	GLuint Program::getUniformLocation(const std::string& location) const
	{
		return glGetUniformLocation(program, location.data());
	}

	void Program::setUniformValue(GLuint location, const glm::vec4& vector) const
	{
		glUniform4fv(location, 1, glm::value_ptr(vector));
	}
}