#pragma once
#include <GL/Program.hpp>
#include <array>
#include "Vertex.hpp"

class ShaderManager
{
public:

	ShaderManager();

	void initialise();
	void useProgram(GL::ProgramType type) const;
	const GL::Program& getProgram(GL::ProgramType type) const;

	glm::mat4 getMVPMatrix() const;
	void setMVPMatrix(glm::mat4 mvp);

private:
	
	GLuint sharedUniformsBindingIndex;
	GLuint sharedUniformsBufferObject;
	glm::mat4 mvpMatrix;
	std::array<GL::Program, GL::ProgramType::COUNT> programs;

};