#pragma once

#include <Box2D/Common/b2Draw.h>
#include <vector>
#include "Geometry.hpp"

class Box2DDebugDraw : public b2Draw
{
public:

	Box2DDebugDraw(float simulationScale)
		: scale(simulationScale)
	{

	}

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) override;

private:

	std::vector<glm::vec2> scaledPts;
	float scale = 1.0f;

	
};