#include "GeometryGenerator.hpp"

GeometryGenerator::GeometryGenerator(Grid generatorGrid, float circleRadius)
	: grid(generatorGrid)
{
	intersectionPoints.reserve(allocationBlockSize);
	intersectionLines.reserve(128);
}

void GeometryGenerator::findInterSectionPoints(const std::vector<Point>& points, float radius)
{
	intersectionPoints.clear();
	float radiusSquared = radius * radius;
	for (const auto& point : points)
	{
		findIntersectingLines(point, radius);
		for (const auto& line : intersectionLines)
		{
			auto direction = line.end - line.start;
			auto f = point - line.start;

			auto a = glm::dot(direction, direction);
			auto b = glm::dot(direction, f);
			auto c = glm::dot(f, f) - radiusSquared;

			auto p = b / a;
			auto q = c / a;
			auto t = glm::sqrt(p * p - q);
			auto scale1 = -p + t;
			auto scale2 = -p - t;

			intersectionPoints.push_back(line.start - direction * scale1);
			intersectionPoints.push_back(line.start - direction * scale2);
		}
	}
}

void GeometryGenerator::findIntersectingLines(const Point& point, float radius)
{
	intersectionLines.clear();

	auto verticalStride = grid.verticalStride();
	auto horizontalStride = grid.horizontalStride();

	auto minPoint = point - radius;
	auto maxPoint = point + radius;
	auto beginX = glm::ceil(minPoint.x / verticalStride);

	auto endX = glm::floor(maxPoint.x / verticalStride);
	auto beginY = glm::ceil(minPoint.y / horizontalStride);
	auto endY = glm::floor(maxPoint.y / horizontalStride);

	for (float x = beginX; x <= endX; x += 1.f)
	{
		intersectionLines.push_back(grid.getVerticalLine(x));
	}

	for (float y = beginY; y <= endY; y += 1.f)
	{
		intersectionLines.push_back(grid.getHorizontalLine(y));
	}

}