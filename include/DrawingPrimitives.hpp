#pragma once
#include <glm\glm.hpp>
#include "Vertex.hpp"
#include <vector>

class ShaderManager;

namespace DrawingPrimitives
{
	// Initializes the drawing functions with the default shaders.
	void init(const ShaderManager* cache);
	void drawCircles(const std::vector<Vertex>& midPoints, float radius);
	void drawRectangle(glm::vec2 p1, glm::vec2 p2);
	void drawPoints(const std::vector<Vertex>& points);
}