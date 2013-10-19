#include "ShaderManager.hpp"
#include "Util.hpp"
#include <GL/Shader.hpp>
#include <string>

void ShaderManager::useProgram(ProgramType type) const
{
	programs.at(type).use();
}

void ShaderManager::initialise()
{
	std::string vertexShaderStr = util::getFileContent("shaders/vertex.glsl");
	std::string geometryShaderStr = util::getFileContent("shaders/geometry.glsl");
	std::string fragmentShaderStr = util::getFileContent("shaders/fragment.glsl");

	auto vertexShader = GL::Shader::create(GL::Shader::Type::Vertex, vertexShaderStr);
	auto geometryShader = GL::Shader::create(GL::Shader::Type::Geometry, geometryShaderStr);
	auto fragmentShader = GL::Shader::create(GL::Shader::Type::Fragment, fragmentShaderStr);
	
	programs[ProgramDefault] = GL::Program({ vertexShader, fragmentShader });
	programs[ProgramPolygon] = GL::Program({ vertexShader, geometryShader, fragmentShader });

	for (auto& kv : programs)
	{
		auto& program = kv.second;
		program.setMVPUniform(glGetUniformLocation(program.getID(), "mvp"));
	}
}

const GL::Program& ShaderManager::getProgram(ProgramType type) const
{
	return programs.at(type);
}

void ShaderManager::setMVPMatrix(glm::mat4 mvp)
{
	for (auto& kv : programs)
	{
		auto& program = kv.second;
		auto location = program.getMVPUniform();
		program.use();
		glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
		glUseProgram(0);
	}
}