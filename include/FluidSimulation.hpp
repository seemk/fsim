#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <iostream>
class FluidSimulation
{
public:

	virtual void createParticles(size_t horizontalAmount, size_t verticalAmount) = 0;
	virtual void update(float dt) = 0;
	virtual void addParticles(glm::vec2 location, size_t count = 4) = 0;
	virtual size_t getParticleCount() const = 0;
	virtual std::vector<glm::vec2> getParticlePositions() const = 0;
	virtual void setDragging(bool drag) = 0;
	virtual void setInputPosition(glm::vec2 position) = 0;
	virtual void debugDraw() const = 0;
	virtual ~FluidSimulation() { }
};