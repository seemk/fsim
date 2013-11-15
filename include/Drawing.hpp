#pragma once
#include "Geometry.hpp"
#include <vector>

class ShaderManager;

namespace Drawing
{
	// Initializes the drawing functions with the default shaders.
	void init(const ShaderManager* cache);

	void drawPoints(const std::vector<glm::vec2>& points);
	void drawCircles(const std::vector<glm::vec2>& midPoints, float radius, int segments = 18, bool filled = false);
	void drawGrid(glm::vec2 start, glm::vec2 end, size_t rows, size_t cols);
	void drawLines(const std::vector<Line>& lines);
	void drawBox(const Point& bottomLeft, const Point& topRight);

	void setColor(glm::vec4 color);
}