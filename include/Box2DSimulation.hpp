#pragma once

#include "FluidSimulation.hpp"
#include "Array2D.hpp"
#include <Box2D/Dynamics/b2World.h>
#include <vector>
#include <memory>

class b2World;

class Box2DSimulation : public FluidSimulation
{

private:

	struct Particle
	{
		glm::vec2 position{ 0.0f, 0.0f };
		glm::vec2 prevPosition{ 0.0f, 0.0f };
		glm::vec2 velocity{ 0.0f, 0.0f };
		bool living = false;
		size_t neighbourCount = 0;
		size_t gridX = 0;
		size_t gridY = 0;
		size_t index = 0;
		size_t fixturesToTest = 0;
		float pressure = 0.0f;
		float pnear = 0.0f;
		std::vector<size_t> neighbourIndices;
	};

	int velocityIterations = 8;
	int positionIterations = 8;
	std::unique_ptr<b2World> physicsWorld;

	std::vector<Particle> particles;
	std::vector<size_t> aliveParticleIndices;
	// Grid of indices
	Array2D<size_t> grid;
	std::vector<glm::vec2> scaledPositions;
	std::vector<glm::vec2> scaledVelocities;
	std::vector<glm::vec2> deltas;

	b2AABB worldBounds;
	
	const float simulationRadius = 0.6f;
	const float idealRadius = 50.f;
	const float idealRadiusSquared = idealRadius * idealRadius;
	const float multiplier = idealRadius / simulationRadius;
	const float viscosity = 0.004f;
	const float cellSize = 0.6f;

	size_t convertToGridIndex(float x) const { return static_cast<size_t>(glm::floor(x / cellSize)); }
	
public:
	Box2DSimulation();
	void createParticles(size_t horizontalAmount, size_t verticalAmount) override;
	void update(float dt) override;
	void addParticle(glm::vec2 location) override;
	size_t getParticleCount() const override;
	std::vector<glm::vec2> getParticlePositions() const override;
	void setDragging(bool drag) override;
	void setInputPosition(glm::vec2 position) override;

	void prepareCollisions();
	void applyLiquidConstraints();
	void prepareSimulation(size_t particleIndex);
	void findNeighbours(Particle& p);

};