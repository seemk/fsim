#include "FluidRenderer.hpp"
#include "FluidSimulator.hpp"
#include "Drawing.hpp"
#include "ShaderManager.hpp"

FluidRenderer::FluidRenderer(int windowWidth, int windowHeight, float scaleFactor,
						float particleRadius, Simulator* simulator, ShaderManager* cache)
	: fluidSimulator(simulator)
	, shaderCache(cache)
	, geometryGen(Grid(80, 45, 1280.f, 720.f), 20.f)
	, width(static_cast<float>(windowWidth))
	, height(static_cast<float>(windowHeight))
	, scale(scaleFactor)
	, particleRadius(particleRadius)
	, gridEnabled(false)
	, particleVertices(simulator->getParticles().size())
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
		Drawing::setColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.25f));
		drawGrid();
	}

	Drawing::setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Drawing::drawPoints(particleVertices);
	Drawing::drawCircles(particleVertices, particleRadius, 36);

	const auto& grid = geometryGen.getGrid();
	Drawing::setColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.25f));
	Drawing::drawGrid(glm::vec2(0.0f, 0.0f), grid.size(), grid.cols(), grid.rows());

	geometryGen.findInterSectionPoints(particleVertices, particleRadius);
	Drawing::setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	glPointSize(2.0f);
	Drawing::drawPoints(geometryGen.getIntersectionPoints());

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