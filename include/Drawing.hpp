#pragma once
#include "Geometry.hpp"
#include "Vertex.hpp"
#include <vector>

class ShaderManager;

namespace Drawing
{
	// Initializes the drawing functions with the default shaders.
	void init(const ShaderManager* cache, int windowWidth, int windowHeight);

	void drawPoints(const std::vector<Vertex>& points);
	void drawGrid(glm::vec2 start, glm::vec2 end, size_t rows, size_t cols);
	void drawLines(const std::vector<Line>& lines);
	void drawBox(const Point& bottomLeft, const Point& topRight);
	void setColor(glm::vec4 color);

}