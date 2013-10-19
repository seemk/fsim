#include "DrawingPrimitives.hpp"
#include <glm\ext.hpp>
#include <GL\glew.h>
#include "ShaderManager.hpp"

namespace DrawingPrimitives
{

	const ShaderManager* shaderCache = nullptr;
	std::vector<Vertex> drawBuffer;
	GLuint vbo;

	void drawPointsInternal(const std::vector<Vertex>& points, ShaderManager::ProgramType t)
	{
		size_t count = points.size();
		std::copy(points.begin(), points.end(), drawBuffer.begin());
		drawBuffer.resize(count);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * count, &drawBuffer[0]);
		shaderCache->useProgram(t);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(GLfloat)));
		glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(drawBuffer.size()));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(0);
	}

	void init(const ShaderManager* cache)
	{
		shaderCache = cache;
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		size_t bufferSize = 50000;
		drawBuffer.reserve(bufferSize);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(Vertex), drawBuffer.data(), GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawRectangle(glm::vec2 p1, glm::vec2 p2)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1.x, p1.y, 0.0f);
		glVertex3f(p2.x, p1.y, 0.0f);
		glVertex3f(p2.x, p2.y, 0.0f);
		glVertex3f(p1.x, p2.y, 0.0f);
		glEnd();
	}

	void drawPoints(const std::vector<Vertex>& points)
	{
		drawPointsInternal(points, ShaderManager::ProgramDefault);
	}

	void drawCircles(const std::vector<Vertex>& midPoints, float radius)
	{
		// TODO: Radius uniform
		drawPointsInternal(midPoints, ShaderManager::ProgramPolygon);
	}

}