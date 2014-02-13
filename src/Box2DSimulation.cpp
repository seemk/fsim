#include "Box2DSimulation.hpp"
#include <Box2D/Box2D.h>
#include <iterator>

// Simulation is a port of https://github.com/klutch/Box2DFluid

class QueryResult : public b2QueryCallback
{
public:
	std::vector<b2Fixture*> fixtures;

	bool ReportFixture(b2Fixture* fixture) override
	{
		fixtures.push_back(fixture);
		return true;
	}
};

Box2DSimulation::Box2DSimulation()
	: physicsWorld(new b2World(b2Vec2{ 0.0f, 9.8f }))
{
	physicsWorld->SetAllowSleeping(true);
}

void Box2DSimulation::findNeighbours(Particle& p)
{
	for (int nx = -1; nx < 2; nx++)
	{
		for (int ny = -1; ny < 2; ny++)
		{
			int x = p.gridX + nx;
			int y = p.gridY + ny;
			if (x < 0 || y < 0) continue;
			size_t gx = static_cast<size_t>(x);
			size_t gy = static_cast<size_t>(y);
			// Check if they are in bounds
			if (gx < grid.cols() && gy < grid.rows())
			{
				size_t neighbourIndex = grid(gx, gy);
				if (neighbourIndex != p.index)
				{
					p.neighbourIndices.push_back(neighbourIndex);
				}
			}
		}
	}
}

void Box2DSimulation::prepareCollisions()
{
	QueryResult queryResult;
	physicsWorld->QueryAABB(&queryResult, worldBounds);

	for (auto fixture : queryResult.fixtures)
	{
		b2AABB aabb;
		b2Transform transform = fixture->GetBody()->GetTransform();
		fixture->GetShape()->ComputeAABB(&aabb, transform, 0);

		// Top left
		size_t Ax = convertToGridIndex(aabb.upperBound.x);
		size_t Ay = convertToGridIndex(aabb.upperBound.y);
		// Bottom right
		size_t Bx = convertToGridIndex(aabb.lowerBound.x) + 1;
		size_t BY = convertToGridIndex(aabb.lowerBound.y) + 1;
	}
}

void Box2DSimulation::prepareSimulation(size_t particleIndex)
{
	auto& particle = particles[particleIndex];

	findNeighbours(particle);
	scaledPositions[particleIndex] = particle.position * multiplier;
	scaledVelocities[particleIndex] = particle.velocity * multiplier;
	deltas[particleIndex] = glm::vec2{ 0.0f, 0.0f };

	particle.prevPosition = glm::vec2{ 0.0f, 0.0f };
	particle.fixturesToTest = 0;
	particle.pressure = 0.0f;
	particle.pnear = 0.0f;
}

void Box2DSimulation::createParticles(size_t horizontalAmount, size_t verticalAmount)
{
	const size_t particleCount = horizontalAmount * verticalAmount;
	particles = std::vector<Particle>{particleCount};
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].index = i;
	}
	grid = Array2D<size_t>{horizontalAmount, verticalAmount, 0};
	aliveParticleIndices = std::vector<size_t>{particleCount};
	deltas = std::vector<glm::vec2>{particleCount};
	scaledPositions = std::vector<glm::vec2>{particleCount};
	scaledVelocities = std::vector<glm::vec2>{particleCount};
}

void Box2DSimulation::applyLiquidConstraints()
{
	for (auto index : aliveParticleIndices)
	{
		const auto& particle = particles[index];

		scaledPositions[index] = particle.position * multiplier;
		scaledVelocities[index] = particle.velocity * multiplier;

		deltas[index] = glm::vec2{ 0.0f, 0.0f };
	}
}

void Box2DSimulation::update(float dt)
{
	for (auto activeIndex : aliveParticleIndices)
	{
		prepareSimulation(activeIndex);
	}
}

void Box2DSimulation::addParticle(glm::vec2 location)
{

}

size_t Box2DSimulation::getParticleCount() const
{
	return particles.size();
}

std::vector<glm::vec2> Box2DSimulation::getParticlePositions() const
{
	std::vector<glm::vec2> positions;
	positions.reserve(getParticleCount());
	std::transform(particles.begin(), particles.end(), std::back_inserter(positions), [](const Particle& p) {
		return p.position;
	});
	return positions;
}

void Box2DSimulation::setDragging(bool drag)
{

}

void Box2DSimulation::setInputPosition(glm::vec2 position)
{

}