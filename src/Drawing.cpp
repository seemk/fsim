#include "Drawing.hpp"
#include <glm/ext.hpp>
#include <GL/glew.h>
#include "ShaderManager.hpp"

namespace Drawing
{

	const ShaderManager* shaderCache = nullptr;
	const size_t bufferSize = 512000 * 4;
	GLuint colorLocation = 0;
	std::array<glm::vec2, bufferSize> drawBuffer;
	std::array<Vertex, bufferSize> vertexBuffer;
	GLuint vertexBufferHandle;
	GLuint frameBufferHandle;
	GLuint vertexAttribObject;
	GLuint textureColorLocation;
	GLuint screenVAO;
	GLuint screenVBO;

	GLfloat screenQuad[] = 
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};

	void init(const ShaderManager* cache)
	{
		shaderCache = cache;
		auto pgm = shaderCache->getProgram(GL::ProgramType::Default);
		pgm.use();
		colorLocation = pgm.getUniformLocation("color");
		pgm.unuse();

		glGenVertexArrays(1, &vertexAttribObject);
		glBindVertexArray(vertexAttribObject);
		glGenBuffers(1, &vertexBufferHandle);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &screenVAO);
		glBindVertexArray(screenVAO);
		glGenBuffers(1, &screenVBO);

		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		auto blurProgram = shaderCache->getProgram(GL::ProgramType::Default);
		blurProgram.use();
		auto loc = blurProgram.getUniformLocation("texFrameBuf");
		blurProgram.setUniformValue(loc, 0);
		blurProgram.unuse();

		glGenFramebuffers(1, &frameBufferHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);

		glGenTextures(1, &textureColorLocation);
		glBindTexture(GL_TEXTURE_2D, textureColorLocation);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorLocation, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void drawPoints(const std::vector<glm::vec2>& points)
	{
		auto program = shaderCache->getProgram(GL::ProgramType::Default);

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
		
		auto program = shaderCache->getProgram(GL::ProgramType::Default);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glDrawArrays(GL_LINES, 0, bufferElements);
		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawBlobs(const std::vector<glm::vec2>& midPoints, float radius, int segments)
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
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * pointCount, &drawBuffer[0]);

		auto program = shaderCache->getProgram(GL::ProgramType::Default);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
		for (size_t i = 0; i < midPoints.size(); ++i)
		{
			glDrawArrays(GL_POLYGON, i*segments, segments);
		}
		glDisableVertexAttribArray(0);
		program.unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(screenVBO), screenQuad);

		auto blurPgm = shaderCache->getProgram(GL::ProgramType::Blur);
		blurPgm.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorLocation);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		blurPgm.unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void drawLines(const std::vector<Line>& lines)
	{
		const size_t vertices = 2 * lines.size();
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * vertices, &lines[0]);

		auto program = shaderCache->getProgram(GL::ProgramType::Default);
		program.use();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glDrawArrays(GL_LINES, 0, vertices);
		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setColor(glm::vec4 color)
	{
		auto pgm = shaderCache->getProgram(GL::ProgramType::Default);
		pgm.use();
		pgm.setUniformValue(colorLocation, color);
		pgm.unuse();
	}

	void drawBox(const Point& bottomLeft, const Point& topRight)
	{
		const size_t segments = 4;
		auto size = topRight - bottomLeft;

		drawBuffer[0] = bottomLeft;
		drawBuffer[1] = Point(bottomLeft.x + size.x, bottomLeft.y);
		drawBuffer[2] = topRight;
		drawBuffer[3] = Point(bottomLeft.x, bottomLeft.y + size.y);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * segments, &drawBuffer[0]);

		auto pgm = shaderCache->getProgram(GL::ProgramType::Default);
		pgm.use();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
		glDrawArrays(GL_LINE_LOOP, 0, segments);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		pgm.unuse();
	}

	void drawBlobs(const std::vector<Vertex>& vertices, float radius, int segments, bool filled)
	{
		size_t pointCount = 0;
		float segmentCount = static_cast<float>(segments);
		for (const auto& v : vertices)
		{
			for (int i = 0; i < segments; ++i)
			{
				float theta = 2.f * glm::pi<float>() * static_cast<float>(i) / segmentCount;
				vertexBuffer[pointCount++] = Vertex(v.position.x + radius * glm::cos(theta),
					v.position.y + radius * glm::sin(theta),
					v.colour.r,
					v.colour.g,
					v.colour.b,
					v.colour.a);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		auto drawMode = filled ? GL_POLYGON : GL_LINE_LOOP;
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * pointCount, &vertexBuffer[0]);

		auto program = shaderCache->getProgram(GL::ProgramType::ColorDefault);
		program.use();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)sizeof(glm::vec2));
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			glDrawArrays(drawMode, i*segments, segments);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		program.unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(screenVBO), screenQuad);

		auto blurPgm = shaderCache->getProgram(GL::ProgramType::Blur);
		blurPgm.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorLocation);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		blurPgm.unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}