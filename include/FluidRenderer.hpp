#pragma once
#include "FluidSimulator.hpp"
#include "Vertex.hpp"

class FluidRenderer
{
public:

	FluidRenderer(int windowWidth, int windowHeight, float scaleFactor, Simulator* simulator);

	void render();
	void updatePositions();

	void enableGrid(bool enable);

private:

	void drawGrid();

	Simulator* fluidSimulator;
	float width;
	float height;
	float scale;
	float pointSize;
	bool gridEnabled;

	std::vector<Vertex> particleVertices;
};