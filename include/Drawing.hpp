#pragma once
#include "Geometry.hpp"
#include "Vertex.hpp"
#include <vector>

class ShaderManager;

namespace Drawing
{
	// Initializes the drawing functions with the default shaders.
	void init(const ShaderManager* cache);

	void drawPoints(const std::vector<glm::vec2>& points);
	void drawBlobs(const std::vector<glm::vec2>& midPoints, float radius, int segments = 18);
	// Vertices passed here are the mid points for the blobs.
	void drawBlobs(const std::vector<Vertex>& vertices, float radius, int segments = 18, bool filled = false);
	void drawGrid(glm::vec2 start, glm::vec2 end, size_t rows, size_t cols);
	void drawLines(const std::vector<Line>& lines);
	void drawBox(const Point& bottomLeft, const Point& topRight);

	void setColor(glm::vec4 color);
}