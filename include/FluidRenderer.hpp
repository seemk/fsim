#pragma once
#include "Vertex.hpp"
#include <vector>
#include <glm/glm.hpp>

class Simulator;
class ShaderManager;

class FluidRenderer
{
public:

	FluidRenderer(int windowWidth, int windowHeight,
				float scaleFactor, float particleRadius,
				Simulator* simulator, ShaderManager* cache);

	void render();
	void updatePositions();
	void setParticleRadius(float radius);
	float getParticleRadius() const;
	void setRenderGridSize(size_t size);
	size_t getRenderGridSize() const;
	void enableGrid(bool enable);

private:

	void drawGrid();

	Simulator* fluidSimulator;
	ShaderManager* shaderCache;

	float width;
	float height;
	float scale;
	float particleRadius;
	size_t renderGridSize;
	bool gridEnabled;

	std::vector<glm::vec2> particleVertices;
};