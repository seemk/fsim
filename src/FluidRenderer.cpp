#include "FluidRenderer.hpp"
#include "DrawingPrimitives.hpp"

FluidRenderer::FluidRenderer(int windowWidth, int windowHeight, float scaleFactor, Simulator* simulator)
	: fluidSimulator(simulator)
	, width(static_cast<float>(windowWidth))
	, height(static_cast<float>(windowHeight))
	, scale(scaleFactor)
	, gridEnabled(false)
	, particleVertices(simulator->getParticles().size())
{

}

void FluidRenderer::render()
{
	updatePositions();
	if (gridEnabled)
	{
		drawGrid();
	}
	DrawingPrimitives::drawCircles(particleVertices, 10.f);
}

void FluidRenderer::drawGrid()
{
	auto grid = fluidSimulator->getGrid();
	for (size_t y = 0; y < grid.cols(); y++)
	{
		for (size_t x = 0; x < grid.rows(); x++)
		{
			glm::vec2 p1(x * scale, height - y * scale);
			glm::vec2 p2(p1.x + width, p1.y + height);
			DrawingPrimitives::drawRectangle(p1, p2);
		}
	}
}

void FluidRenderer::updatePositions()
{
	auto& particles = fluidSimulator->getParticles();
	particleVertices.resize(particles.size());

	std::transform(particles.cbegin(), particles.cend(), particleVertices.begin(), [=](const Particle& p)
	{
		return Vertex{ { p.x * scale, height - p.y * scale }, { 1.0f, 1.0f, 1.0f } };
	});
}

void FluidRenderer::enableGrid(bool enable)
{
	gridEnabled = enable;
}