#pragma once

#include "FluidSimulation.hpp"
#include <Box2D/Dynamics/b2World.h>
#include "Box2DDebugDraw.hpp"
#include "ScreenSize.hpp"
#include <vector>
#include <memory>

class Box2DSimulation : public FluidSimulation
{

private:

	int velocityIterations = 8;
	int positionIterations = 8;
	const float scale = 32.f;
	std::unique_ptr<b2World> physicsWorld;

	Box2DDebugDraw physicsDebugDraw;
	ScreenSize screenSize;
	
public:
	Box2DSimulation(const ScreenSize& windowDimensions);
	void createParticles(size_t horizontalAmount, size_t verticalAmount) override;
	void update(float dt) override;
	void addParticles(glm::vec2 location, size_t count = 4) override;
	size_t getParticleCount() const override;
	std::vector<glm::vec2> getParticlePositions() const override;
	void setDragging(bool drag) override;
	void setInputPosition(glm::vec2 position) override;
	void debugDraw() const override;
	void buildBodies();
};