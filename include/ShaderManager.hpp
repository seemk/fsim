#pragma once
#include <GL/Program.hpp>
#include <glm/glm.hpp>
#include <array>

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
	std::array<GL::Program, 2> programs;

};