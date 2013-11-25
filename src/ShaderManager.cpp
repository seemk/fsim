#include "ShaderManager.hpp"
#include "Util.hpp"
#include <GL/Shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderManager::ShaderManager()
: sharedUniformsBindingIndex(0)
{

}

void ShaderManager::useProgram(GL::ProgramType type) const
{
	programs.at(type).use();
}

void ShaderManager::initialise()
{
	std::string vertexShaderStr = util::getFileContent("shaders/vertex.glsl");
	std::string fragmentShaderStr = util::getFileContent("shaders/fragment.glsl");
	std::string blurShaderVertexStr = util::getFileContent("shaders/fb_blur_vertex.glsl");
	std::string blurShaderFragStr = util::getFileContent("shaders/fb_blur_fragment.glsl");
	std::string vertexColourShaderStr = util::getFileContent("shaders/vertex_colour.glsl");
	std::string primitiveGeometryShaderStr = util::getFileContent("shaders/geometry.glsl");

	auto vertexShader = GL::Shader::create(GL::Shader::Type::Vertex, vertexShaderStr);
	auto fragmentShader = GL::Shader::create(GL::Shader::Type::Fragment, fragmentShaderStr);
	auto blurShaderVertex = GL::Shader::create(GL::Shader::Type::Vertex, blurShaderVertexStr);
	auto blurShaderFrag = GL::Shader::create(GL::Shader::Type::Fragment, blurShaderFragStr);
	auto vertexColourShader = GL::Shader::create(GL::Shader::Type::Vertex, vertexColourShaderStr);
	auto primitiveGeometryShader = GL::Shader::create(GL::Shader::Type::Geometry, primitiveGeometryShaderStr);

	programs[GL::ProgramType::Default] = GL::Program({ vertexShader, fragmentShader });
	programs[GL::ProgramType::ColorDefault] = GL::Program({ vertexColourShader, fragmentShader });
	programs[GL::ProgramType::Blur] = GL::Program({ blurShaderVertex, blurShaderFrag });
	programs[GL::ProgramType::ColorCircles] = GL::Program({ vertexColourShader, primitiveGeometryShader, fragmentShader });

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

const GL::Program& ShaderManager::getProgram(GL::ProgramType type) const
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