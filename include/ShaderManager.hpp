#pragma once
#include <GL/Program.hpp>
#include <glm/glm.hpp>
#include <array>

class ShaderManager
{
public:

	enum ProgramType
	{
		ProgramDefault, // The default shader which just colours the vertices
		ProgramPolygon // A geometry shader used to draw arbitrary polygons
	};

	ShaderManager();

	void initialise();
	void useProgram(ProgramType type) const;
	const GL::Program& getProgram(ProgramType type) const;

	glm::mat4 getMVPMatrix() const;
	void setMVPMatrix(glm::mat4 mvp);

private:
	
	GLuint sharedUniformsBindingIndex;
	GLuint sharedUniformsBufferObject;
	glm::mat4 mvpMatrix;
	std::array<GL::Program, 1> programs;
};