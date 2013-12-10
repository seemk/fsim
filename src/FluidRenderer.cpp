#include "FluidRenderer.hpp"
#include "FluidSimulator.hpp"
#include "Drawing.hpp"
#include "ShaderManager.hpp"

static const GLfloat screenQuad[] =
{
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f
};

FluidRenderer::FluidRenderer(int windowWidth, int windowHeight, float scaleFactor,
	float particleRadius, Simulator* simulator, ShaderManager* cache)
	: fluidSimulator(simulator)
	, shaderCache(cache)
	, screenSize(windowWidth, windowHeight)
	, scale(scaleFactor)
	, particleRadius(particleRadius)
	, blurred(true)
	, blobInterpolation(true)
	, gridEnabled(false)
	, particleVertices(simulator->getParticles().size())
	, particlePositions(false)
	, pointCount(0)
{

	auto& particles = fluidSimulator->getParticles();
	for (auto& particle : particles)
	{
			particle.r = 255;
			particle.g = 255;
			particle.b = 255;
	}

	setupBuffers();
}

void FluidRenderer::setParticleRadius(float radius)
{
	particleRadius = radius;
}

void FluidRenderer::render()
{
	updatePositions();
	if (gridEnabled)
	{
		Drawing::setColor(glm::vec4(0.392f, 0.8, 0.2f, 1.0f));
		drawGrid();
	}


	Drawing::setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	const int segments = 18;
	generateCircleVertices(segments);

	glBindFramebuffer(GL_FRAMEBUFFER, lowResBufferHandle);
	
	glViewport(0, 0, screenSize.iWidth / textureScale, screenSize.iHeight / textureScale);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)* pointCount, &vertexBuffer[0]);

	auto program = shaderCache->getProgram(GL::ProgramType::ColorDefault);
	program.use();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)sizeof(glm::vec2));
	for (size_t i = 0; i < particleVertices.size(); ++i)
	{
		glDrawArrays(GL_POLYGON, i*segments, segments);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	program.unuse();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (blurred) applyBlur(lowResTextureHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, screenSize.iWidth, screenSize.iHeight);

	if (blobInterpolation)
	{
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		auto filterPgm = shaderCache->getProgram(GL::ProgramType::TexCoordThreshold);
		filterPgm.use();
		glBindTexture(GL_TEXTURE_2D, lowResTextureHandle);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		filterPgm.unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, lowResBufferHandle);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, screenSize.iWidth / textureScale, screenSize.iHeight / textureScale,
			0, 0, screenSize.iWidth, screenSize.iHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, offScreenBufferTextureHandle);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, screenSize.iWidth, screenSize.iHeight, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (particlePositions)
	{
		glPointSize(3.0f);
		Drawing::setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Drawing::drawPoints(particleVertices);
		
	}
}

void FluidRenderer::generateCircleVertices(int segments)
{
	const float segmentCount = static_cast<float>(segments);

	pointCount = 0;
	for (const auto& v : particleVertices)
	{
		for (int i = 0; i < segments; ++i)
		{
			float theta = 2.f * glm::pi<float>() * static_cast<float>(i) / segmentCount;
			vertexBuffer[pointCount++] = Vertex(v.position.x + particleRadius * glm::cos(theta),
				v.position.y + particleRadius * glm::sin(theta),
				v.colour.r,
				v.colour.g,
				v.colour.b,
				v.colour.a);
		}
	}
}

void FluidRenderer::applyBlur(GLuint textureID)
{
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(screenVBO), screenQuad);

	auto blurPgm = shaderCache->getProgram(GL::ProgramType::Blur);
	blurPgm.use();
	glBindTexture(GL_TEXTURE_2D, textureID);
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

void FluidRenderer::setupBuffers()
{
	glGenFramebuffers(1, &lowResBufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, lowResBufferHandle);

	glGenTextures(1, &lowResTextureHandle);
	glBindTexture(GL_TEXTURE_2D, lowResTextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.iWidth / textureScale, screenSize.iHeight / textureScale, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lowResTextureHandle, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Screen quad buffer
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glGenBuffers(1, &screenVBO);

	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Vertex buffer objects
	glGenVertexArrays(1, &vertexAttribObject);
	glBindVertexArray(vertexAttribObject);
	glGenBuffers(1, &vertexBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Offscreen buffer

	auto blurProgram = shaderCache->getProgram(GL::ProgramType::Default);
	blurProgram.use();
	auto loc = blurProgram.getUniformLocation("texFrameBuf");
	blurProgram.setUniformValue(loc, 0);
	blurProgram.unuse();
	glGenFramebuffers(1, &offScreenBufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenBufferHandle);
	glGenTextures(1, &offScreenBufferTextureHandle);
	glBindTexture(GL_TEXTURE_2D, offScreenBufferTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.iWidth, screenSize.iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offScreenBufferTextureHandle, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FluidRenderer::drawGrid()
{
	auto grid = fluidSimulator->getGrid();

	auto rows = grid.rows();
	auto cols = grid.cols();
	auto width = rows * scale;
	auto height = cols * scale;
	Drawing::drawGrid(glm::vec2(0.0f, 0.0f), glm::vec2(width, height), cols, rows);

}

void FluidRenderer::updatePositions()
{
	auto& particles = fluidSimulator->getParticles();
	particleVertices.resize(particles.size());

	std::transform(particles.cbegin(), particles.cend(), particleVertices.begin(), [=](const Particle& p)
	{

		return Vertex{ p.x * scale, p.y * scale, p.r, p.g, p.b, 255 };
	});
}

void FluidRenderer::enableGrid(bool enable)
{
	gridEnabled = enable;
}

float FluidRenderer::getParticleRadius() const
{
	return particleRadius;
}

void FluidRenderer::toggleBlur()
{
	blurred = !blurred;
}