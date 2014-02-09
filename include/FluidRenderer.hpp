#pragma once
#include <Geometry.hpp>
#include "Vertex.hpp"
#include <vector>
#include <array>

class Simulator;
class ShaderManager;

struct ScreenSize
{

	ScreenSize(int screenWidth, int screenHeight)
	: width(static_cast<float>(screenWidth))
	, height(static_cast<float>(screenHeight))
	, iWidth(screenWidth)
	, iHeight(screenHeight)
	{

	}

	float width;
	float height;
	int iWidth;
	int iHeight;

};

class FluidRenderer
{
public:

	FluidRenderer(int windowWidth, int windowHeight,
				float scaleFactor, float particleRadius,
				Simulator* simulator, ShaderManager* cache);

	void render(const std::vector<Vertex>& vertices);
	void updatePositions();
	void setParticleRadius(float radius);
	float getParticleRadius() const;
	void toggleBlur();
	void toggleBlobInterpolation() { blobInterpolation = !blobInterpolation; }
	void togglePositionRendering() { particlePositions = !particlePositions; }
	const std::vector<Vertex>& getSimulatorVertices() const { return particleVertices; }
	float getScale() const { return scale; }
	void setColorCutting(bool colorCutting);

private:

	void setupBuffers();
	void generateCircleVertices(const std::vector<Vertex>& vertices, int segments);
	void applyBlur(GLuint textureID);

	Simulator* fluidSimulator;
	ShaderManager* shaderCache;

	ScreenSize screenSize;
	float scale;
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

	std::vector<Vertex> particleVertices;
	std::array<Vertex, 512000 * 8> vertexBuffer;
};