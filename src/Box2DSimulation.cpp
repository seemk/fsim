#include "Box2DSimulation.hpp"
#include <Box2D/Box2D.h>
#include <glm/gtx/norm.hpp>
#include <iterator>


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

Box2DSimulation::Box2DSimulation(const ScreenSize& windowDimensions)
	: physicsWorld(std::make_unique<b2World>(b2Vec2{ 0.0f, 0.8f }))
	, physicsDebugDraw(scale)
	, screenSize(windowDimensions)
{
	physicsWorld->SetAllowSleeping(true);
	
	physicsWorld->SetDebugDraw(&physicsDebugDraw);
	physicsWorld->SetParticleRadius(0.2f);
	physicsWorld->SetParticleDamping(0.8f);
	physicsDebugDraw.SetFlags(b2Draw::e_shapeBit);

	buildBodies();
}


void Box2DSimulation::createParticles(size_t horizontalAmount, size_t verticalAmount)
{
}


void Box2DSimulation::update(float dt)
{
	physicsWorld->Step(dt, velocityIterations, positionIterations);

}

void Box2DSimulation::debugDraw() const
{
	physicsWorld->DrawDebugData();
}

void Box2DSimulation::addParticles(glm::vec2 location, size_t count)
{

}

size_t Box2DSimulation::getParticleCount() const
{
	return physicsWorld->GetParticleCount();
}

std::vector<glm::vec2> Box2DSimulation::getParticlePositions() const
{
	auto numParticles = physicsWorld->GetParticleCount();
	auto particleBuffer = physicsWorld->GetParticlePositionBuffer();
	std::vector<glm::vec2> positions;
	positions.reserve(numParticles);

	std::transform(particleBuffer, particleBuffer + numParticles, std::back_inserter(positions), [=](const b2Vec2& pos)
	{
		return glm::vec2(pos.x * scale, pos.y * scale);
	});

	return positions;
}

void Box2DSimulation::setDragging(bool drag)
{

}

void Box2DSimulation::setInputPosition(glm::vec2 position)
{

}

void Box2DSimulation::buildBodies()
{

	// Edges
	{
		b2BodyDef bd;
		b2Body* fluidBody = physicsWorld->CreateBody(&bd);

		b2ChainShape shape;
		const b2Vec2 verts[] =
		{
			{ 0.0f, 0.0f },
			{ 0.0f, screenSize.height / scale },
			{ screenSize.width / scale, screenSize.height / scale },
			{ screenSize.width / scale, 0.0f }
		};
		shape.CreateLoop(verts, 4);
		fluidBody->CreateFixture(&shape, 0.0f);

	}

	// Fluid
	{
		b2PolygonShape shape;
		shape.SetAsBox(400.f / scale, 150.f / scale, b2Vec2((screenSize.width / 2.0f) / scale, (screenSize.height * 0.25f) / scale), 0);
		b2ParticleGroupDef pd;
		pd.shape = &shape;
		physicsWorld->CreateParticleGroup(pd);
	}

	// Squares
	const std::vector<b2Vec2> squarePositions =
	{
		b2Vec2((screenSize.width * 0.5f) / scale, (screenSize.height * 0.9f) / scale),
		b2Vec2((screenSize.width * 0.2f) / scale, (screenSize.height * 0.9f) / scale)
	};

	for (const auto& pos : squarePositions)
	{
		b2BodyDef bd;
		b2Body* square = physicsWorld->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(100.f / scale, 100.f / scale, pos, 0);

		square->CreateFixture(&shape, 0.0f);
	}

	// Circles
	const std::vector<b2Vec2> centers = 
	{
		b2Vec2((screenSize.width * 0.8f) / scale, (screenSize.height * 0.9f) / scale),
		b2Vec2((screenSize.width * 0.35f) / scale, (screenSize.height * 0.8f) / scale)
	};

	for (const auto& center : centers)
	{
		b2BodyDef bd;
		b2Body* circle = physicsWorld->CreateBody(&bd);

		b2CircleShape shape;
		shape.m_p = center;
		shape.m_radius = 50.f / scale;
		circle->CreateFixture(&shape, 0.0f);
	}
}