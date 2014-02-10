#include "Box2DSimulation.hpp"
#include <Box2D/Box2D.h>

Box2DSimulation::Box2DSimulation()
	: physicsWorld(new b2World(b2Vec2{ 0.0f, 9.8f }))
{
	physicsWorld->SetAllowSleeping(true);
}

void Box2DSimulation::createParticles(size_t horizontalAmount, size_t verticalAmount)
{

}

void Box2DSimulation::update(float dt)
{
	physicsWorld->Step(dt, velocityIterations, positionIterations);
}

void Box2DSimulation::addParticle(glm::vec2 location)
{

}

size_t Box2DSimulation::getParticleCount() const
{
	return 0;
}

std::vector<glm::vec2> Box2DSimulation::getParticlePositions() const
{
	return std::vector<glm::vec2>{};
}

void Box2DSimulation::setDragging(bool drag)
{

}

void Box2DSimulation::setInputPosition(glm::vec2 position)
{

}