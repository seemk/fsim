#include "Drawing.hpp"
#include <glm/ext.hpp>
#include <GL/glew.h>
#include "ShaderManager.hpp"
#include <algorithm>

namespace Drawing
{

	int screenWidth = 0;
	int screenHeight = 0;

	const ShaderManager* shaderCache = nullptr;
	const size_t bufferSize = 512000 * 4;
	GLuint colorLocation = 0;
	std::array<glm::vec2, bufferSize> drawBuffer;
	GLuint drawBufferVBO;
	GLuint drawBufferVAO;

	void init(const ShaderManager* cache, int windowWidth, int windowHeight)
	{
		shaderCache = cache;
		screenWidth = windowWidth;
		screenHeight = windowHeight;
		auto pgm = shaderCache->getProgram(GL::ProgramType::Default);
		pgm.use();
		colorLocation = pgm.getUniformLocation("color");
		pgm.unuse();

		glGenVertexArrays(1, &drawBufferVAO);
		glBindVertexArray(drawBufferVAO);
		glGenBuffers(1, &drawBufferVBO);

		glBindBuffer(GL_ARRAY_BUFFER, drawBufferVBO);
		glBufferData(GL_ARRAY_BUFFER, drawBuffer.size() * sizeof(float), nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void drawPoints(const Point* pts, size_t count)
	{
		//std::transform(points.begin(), points.end(), drawBuffer.begin(), [](const Vertex& v)
		//{
		//	return v.position;
		//});
		auto program = shaderCache->getProgram(GL::ProgramType::Default);
		
		glBindBuffer(GL_ARRAY_BUFFER, drawBufferVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * count, pts);

		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(count));
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

		glBindBuffer(GL_ARRAY_BUFFER, drawBufferVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * bufferElements, &drawBuffer[0]);
		
		auto program = shaderCache->getProgram(GL::ProgramType::Default);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glDrawArrays(GL_LINES, 0, bufferElements);
		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawLines(const Line* lines, size_t count)
	{
		const size_t vertices = 2 * count;
		glBindBuffer(GL_ARRAY_BUFFER, drawBufferVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * vertices, lines);

		auto program = shaderCache->getProgram(GL::ProgramType::Default);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glDrawArrays(GL_LINES, 0, vertices);
		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setColor(float r, float g, float b, float a)
	{
		auto pgm = shaderCache->getProgram(GL::ProgramType::Default);
		pgm.use();
		pgm.setUniformValue(colorLocation, glm::vec4(r, g, b, a));
		pgm.unuse();
	}

	void drawBox(const Point& topLeft, const Point& size)
	{
		const size_t segments = 4;
		const Point pts[segments] = { topLeft,
			Point(topLeft.x, topLeft.y + size.y),
			Point(topLeft.x + size.x, topLeft.y + size.y),
			Point(topLeft.x + size.x, topLeft.y) };

		drawPolygon(pts, segments);
	}

	void drawPolygon(const Point* pts, size_t count)
	{

		glBindBuffer(GL_ARRAY_BUFFER, drawBufferVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * count, pts);

		auto pgm = shaderCache->getProgram(GL::ProgramType::Default);
		pgm.use();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
		glDrawArrays(GL_LINE_LOOP, 0, count);
		pgm.unuse();
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		
	}

}