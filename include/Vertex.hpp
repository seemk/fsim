#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct Vertex
{

	Vertex() = default;

	Vertex(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	: position(glm::vec2(x, y))
	, colour(glm::u8vec4(r, g, b, a))
	{

	}

	glm::vec2 position;
	glm::u8vec4 colour;
};