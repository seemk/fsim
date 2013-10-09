#pragma once
#include <glm\glm.hpp>

namespace DrawingPrimitives
{
	void drawCircle(glm::vec2 center, float r, int segments = 8);
	void drawRectangle(glm::vec2 p1, glm::vec2 p2);
	void drawFilledRectangle(glm::vec2 p1, glm::vec2 p2);
}