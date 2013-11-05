#include "ShaderManager.hpp"
#include "Util.hpp"
#include <GL/Shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

ShaderManager::ShaderManager()
: sharedUniformsBindingIndex(0)
{

}

void ShaderManager::useProgram(ProgramType type) const
{
	programs.at(type).use();
}

void ShaderManager::initialise()
{
	std::string vertexShaderStr = util::getFileContent("shaders/vertex.glsl");
	std::string fragmentShaderStr = util::getFileContent("shaders/fragment.glsl");

	auto vertexShader = GL::Shader::create(GL::Shader::Type::Vertex, vertexShaderStr);
	auto fragmentShader = GL::Shader::create(GL::Shader::Type::Fragment, fragmentShaderStr);

	programs[ProgramDefault] = GL::Program({ vertexShader, fragmentShader });

	for (auto& pgm : programs)
	{
		glUniformBlockBinding(pgm.getID(), pgm.getSharedUniformBlockIndex(), sharedUniformsBindingIndex);
	}

	glGenBuffers(1, &sharedUniformsBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, sharedUniformsBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, sharedUniformsBindingIndex, sharedUniformsBufferObject, 0, sizeof(glm::mat4));

}

const GL::Program& ShaderManager::getProgram(ProgramType type) const
{
	return programs.at(type);
}

void ShaderManager::setMVPMatrix(glm::mat4 mvp)
{
	mvpMatrix = mvp;
	glBindBuffer(GL_UNIFORM_BUFFER, sharedUniformsBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mvp));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glm::mat4 ShaderManager::getMVPMatrix() const
{
	return mvpMatrix;
}