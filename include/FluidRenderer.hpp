#pragma once
#include <Geometry.hpp>
#include "Vertex.hpp"
#include "ScreenSize.hpp"
#include <vector>
#include <array>

class FluidSimulation;
class ShaderManager;

class FluidRenderer
{
public:

	FluidRenderer(int windowWidth, int windowHeight, float particleRadius,
				const FluidSimulation* simulation, ShaderManager* cache);

	void setSimulation(const FluidSimulation* simulation);
	void render();
	void updatePositions();
	void setParticleRadius(float radius);
	float getParticleRadius() const;
	void toggleBlur();
	void toggleBlobInterpolation() { blobInterpolation = !blobInterpolation; }
	void togglePositionRendering() { particlePositions = !particlePositions; }
	void setColorCutting(bool colorCutting);

private:

	void setupBuffers();
	void generateCircleVertices(const std::vector<Vertex>& vertices, int segments);
	void applyBlur(GLuint textureID);

	const FluidSimulation* fluidSimulation;
	ShaderManager* shaderCache;

	ScreenSize screenSize;
	float particleRadius;
	bool particlePositions;
	bool blurred;
	bool blobInterpolation;
	GLuint lowResBufferHandle;
	GLuint lowResTextureHandle;
	GLuint screenVAO;
	GLuint screenVBO;
	GLuint vertexBufferHandle;
	GLuint vertexAttribObject;
	GLuint offScreenBufferHandle;
	GLuint offScreenBufferTextureHandle;
	size_t pointCount;
	const int textureScale = 4;

	std::vector<glm::vec2> positions;
	std::vector<Vertex> particleVertices;
	std::array<Vertex, 512000 * 8> vertexBuffer;
};