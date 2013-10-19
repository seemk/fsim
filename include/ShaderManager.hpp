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

	void initialise();
	void useProgram(ProgramType type) const;
	const GL::Program& getProgram(ProgramType type) const;
	void setMVPMatrix(glm::mat4 mvp);

private:

	std::array<GL::Program, 2> programs;
};