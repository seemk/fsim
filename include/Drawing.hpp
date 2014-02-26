#pragma once
#include "Geometry.hpp"
#include "Vertex.hpp"
#include <vector>

class ShaderManager;

namespace Drawing
{
	// Initializes the drawing functions with the default shaders.
	void init(const ShaderManager* cache, int windowWidth, int windowHeight);

	void drawPoints(const Point* pts, size_t count);
	void drawGrid(glm::vec2 start, glm::vec2 end, size_t rows, size_t cols);
	void drawLines(const Line* lines, size_t count);
	void drawBox(const Point& topLeft, const Point& size);
	void drawPolygon(const Point* pts, size_t count);
	void setColor(float r, float g, float b, float a);

}