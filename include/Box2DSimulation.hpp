#pragma once

#include "FluidSimulation.hpp"
#include <Box2D/Dynamics/b2World.h>
#include <vector>
#include <memory>

class b2World;

class Box2DSimulation : public FluidSimulation
{
public:
	Box2DSimulation();
	void createParticles(size_t horizontalAmount, size_t verticalAmount) override;
	void update(float dt) override;
	void addParticle(glm::vec2 location) override;
	size_t getParticleCount() const override;
	std::vector<glm::vec2> getParticlePositions() const override;
	void setDragging(bool drag) override;
	void setInputPosition(glm::vec2 position) override;

private:

	int velocityIterations = 8;
	int positionIterations = 8;
	std::unique_ptr<b2World> physicsWorld;

};