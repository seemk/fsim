#include "DrawingPrimitives.hpp"
#include <cmath>
#include <GLFW\glfw3.h>
#include <glm\gtx\constants.hpp>
namespace DrawingPrimitives
{
	void drawCircle(glm::vec2 center, float r, int segments)
	{
		float theta = 2.f * glm::pi<float>() / static_cast<float>(segments);
		float c = std::cosf(theta);
		float s = std::sinf(theta);
		float x = r;
		float y = 0.0f;
		float t = 0.0f;
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < segments; ++i)
		{
			glVertex2f(x + center.x, y + center.y);
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		}
		glEnd();
	}

	void drawRectangle(glm::vec2 p1, glm::vec2 p2)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1.x, p1.y, 0.0f);
		glVertex3f(p2.x, p1.y, 0.0f);
		glVertex3f(p2.x, p2.y, 0.0f);
		glVertex3f(p1.x, p2.y, 0.0f);
		glEnd();
	}

	void drawFilledRectangle(glm::vec2 p1, glm::vec2 p2)
	{
		
	}
}