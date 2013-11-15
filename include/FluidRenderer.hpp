#pragma once
#include <Geometry.hpp>
#include <vector>

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
	void enableGrid(bool enable);

private:

	void drawGrid();

	Simulator* fluidSimulator;
	ShaderManager* shaderCache;

	float width;
	float height;
	float scale;
	float particleRadius;
	bool gridEnabled;

	std::vector<glm::vec2> particleVertices;
};