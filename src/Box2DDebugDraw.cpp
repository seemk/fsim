#include "Box2DDebugDraw.hpp"
#include "Drawing.hpp"
#include <algorithm>

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	scaledPts.clear();
	Drawing::setColor(color.r, color.g, color.b, 1.0f);
	std::transform(vertices, vertices + vertexCount, std::back_inserter(scaledPts), [=](const b2Vec2& p)
	{
		return Point(p.x * scale, p.y * scale);
	});
	Drawing::drawPolygon(scaledPts.data(), scaledPts.size());
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	scaledPts.clear();
	const float segments = 16.f;
	float scaledRadius = radius * scale;
	glm::vec2 scaledCenter{ center.x * scale, center.y * scale };

	for (float i = 0.0f; i < segments; i += 1.0f)
	{
		float theta = 2.f * glm::pi<float>() * i / segments;
		scaledPts.emplace_back(scaledCenter.x + scaledRadius * glm::cos(theta),
			scaledCenter.y + scaledRadius * glm::sin(theta));
	}
	Drawing::setColor(color.r, color.g, color.b, 1.0f);
	Drawing::drawPolygon(scaledPts.data(), scaledPts.size());
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	DrawCircle(center, radius, color);
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{

}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
{

}

void Box2DDebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count)
{

}