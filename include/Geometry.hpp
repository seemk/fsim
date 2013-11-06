#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

typedef glm::vec2 Point;

struct Line
{
	glm::vec2 start;
	glm::vec2 end;
};

struct Circle
{
	glm::vec2 center;
	float radius;
};

struct Vertex
{
	GLfloat position[2];
	GLfloat color[3];
};

class Grid
{

public:

	Grid(size_t gridRows, size_t gridCols, float gridWidth, float gridHeight)
		: m_rows(gridRows)
		, m_columns(gridCols)
		, m_width(gridWidth)
		, m_height(gridHeight)
	{

	}

	glm::vec2 size() const { return glm::vec2(m_width, m_height); }
	float horizontalStride() const { return m_height / static_cast<float>(m_columns); }
	float verticalStride() const { return m_width / static_cast<float>(m_rows); }

	Line getVerticalLine(float column) const
	{
		float horizontalOffset = column * verticalStride();
		glm::vec2 p1(horizontalOffset, 0.0f);
		glm::vec2 p2(horizontalOffset, m_height);
		return Line{ p1, p2 };
	}

	Line getHorizontalLine(float row) const
	{
		float verticalOffset = row * horizontalStride();
		glm::vec2 p1(0.0f, verticalOffset);
		glm::vec2 p2(m_width, verticalOffset);
		return Line{ p1, p2 };
	}

	size_t rows() const { return m_rows; }
	size_t cols() const { return m_columns; }
	float width() const { return m_width; }
	float height() const { return m_height; }

private:

	size_t m_rows;
	size_t m_columns;
	float m_width;
	float m_height;

};
