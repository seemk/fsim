#include "FluidRenderer.hpp"
#include "FluidSimulator.hpp"
#include "Drawing.hpp"
#include "ShaderManager.hpp"

FluidRenderer::FluidRenderer(int windowWidth, int windowHeight, float scaleFactor,
	float particleRadius, Simulator* simulator, ShaderManager* cache)
	: fluidSimulator(simulator)
	, shaderCache(cache)
	, width(static_cast<float>(windowWidth))
	, height(static_cast<float>(windowHeight))
	, scale(scaleFactor)
	, particleRadius(particleRadius)
	, gridEnabled(false)
	, particleVertices(simulator->getParticles().size())
	, particlePositions(false)
{

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


	Drawing::setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//Drawing::setColor(glm::vec4(0.1f, 0.3f, 1.0f, 1.0f));
	Drawing::drawBlobs(particleVertices, particleRadius, 18);

	if (particlePositions)
	{
		glPointSize(3.0f);
		Drawing::setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Drawing::drawPoints(particleVertices);
	}

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
		return glm::vec2{ p.x * scale, p.y * scale };
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