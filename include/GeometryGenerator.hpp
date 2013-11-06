#pragma once
#include "Geometry.hpp"
#include <vector>

class GeometryGenerator
{
public:

	GeometryGenerator(Grid generatorGrid, float circleRadius);

	void findIntersectingLines(const Point& point, float radius);
	void findInterSectionPoints(const std::vector<Point>& points, float radius);

	const Grid& getGrid() const { return grid; }

	const std::vector<Point>& getIntersectionPoints() const { return intersectionPoints; }

private:

	std::vector<Point> intersectionPoints;
	std::vector<Line> intersectionLines;
	Grid grid;
	float radius;

	const size_t allocationBlockSize = 512000;

};