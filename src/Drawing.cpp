#include "Drawing.hpp"
#include <glm/ext.hpp>
#include <GL/glew.h>
#include "ShaderManager.hpp"

namespace Drawing
{

	const ShaderManager* shaderCache = nullptr;
	const size_t bufferSize = 512000;
	GLuint colorLocation = 0;
	// Preallocates bufferSize * 2 * 4 bytes
	std::array<glm::vec2, bufferSize> drawBuffer;
	GLuint vertexBufferHandle;


	void init(const ShaderManager* cache)
	{
		shaderCache = cache;
		colorLocation = shaderCache->getProgram(ShaderManager::ProgramDefault).getUniformLocation("color");
		GLuint vertexAttribObject;
		glGenVertexArrays(1, &vertexAttribObject);
		glBindVertexArray(vertexAttribObject);

		glGenBuffers(1, &vertexBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawPoints(const std::vector<glm::vec2>& points)
	{
		auto program = shaderCache->getProgram(ShaderManager::ProgramDefault);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * points.size(), &points[0]);

		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(points.size()));
		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawGrid(glm::vec2 start, glm::vec2 end, size_t rows, size_t cols)
	{
		float colOffset = (end.x - start.x) / static_cast<float>(cols);
		glm::vec2 col1 = start;
		glm::vec2 col2(start.x, end.y);
		size_t columnVertexCount = cols * 2;
		size_t rowVertexCount = rows * 2;
		size_t bufferElements = columnVertexCount + rowVertexCount;
		for (size_t i = 0; i <= columnVertexCount; i += 2)
		{
			col1.x += colOffset;
			col2.x += colOffset;
			drawBuffer[i] = glm::vec2(col1.x, col1.y);
			drawBuffer[i + 1] = glm::vec2(col2.x, col2.y);
		}
		col1 = start;
		col2.x = end.x;
		col2.y = start.y;
		colOffset = (end.y - start.y) / static_cast<float>(rows);

		for (size_t i = columnVertexCount; i <= bufferElements; i += 2)
		{
			col1.y += colOffset;
			col2.y += colOffset;
			drawBuffer[i] = glm::vec2(col1.x, col1.y);
			drawBuffer[i + 1] = glm::vec2(col2.x, col2.y);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * bufferElements, &drawBuffer[0]);
		
		auto program = shaderCache->getProgram(ShaderManager::ProgramDefault);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glDrawArrays(GL_LINES, 0, bufferElements);
		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawCircles(const std::vector<glm::vec2>& midPoints, float radius, int segments)
	{
		size_t pointCount = 0;
		float segmentCount = static_cast<float>(segments);
		for (const auto& p : midPoints)
		{
			for (int i = 0; i < segments; ++i)
			{
				float theta = 2.f * glm::pi<float>() * static_cast<float>(i) / segmentCount;
				drawBuffer[pointCount++] = glm::vec2(p.x + radius * glm::cos(theta),
					p.y + radius * glm::sin(theta));
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * pointCount, &drawBuffer[0]);
		
		auto program = shaderCache->getProgram(ShaderManager::ProgramDefault);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
		for (size_t i = 0; i < midPoints.size(); ++i)
		{
			glDrawArrays(GL_LINE_LOOP, i*segments, segments);
		}
		glDisableVertexAttribArray(0);
		program.unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setColor(glm::vec4 color)
	{
		auto pgm = shaderCache->getProgram(ShaderManager::ProgramDefault);
		pgm.use();
		pgm.setUniformValue(colorLocation, color);
		pgm.unuse();
	}

}