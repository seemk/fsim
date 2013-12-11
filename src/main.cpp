#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "FluidSimulator.hpp"
#include "FluidRenderer.hpp"
#include "Drawing.hpp"
#include "ShaderManager.hpp"
#include "AtoDemo.hpp"
#include <chrono>
#include <iostream>

enum Demo : int
{
	AtoMorph,
	Fluid
};

struct Mouse
{
	float x;
	float y;
	bool pressed;
};

Mouse mouse = { 0.0f, 0.0f, false };
const int windowWidth	= 1280;
const int windowHeight	= 720;
const int particlesX	= 100;
const int particlesY	= 50;
const float particleIncrement = 2.0f;
const int demoCount = 2;
int currentDemo = Fluid;
bool paused = false;

bool addingParticles	= false;
float scaleFactor		= 8.f;

static ShaderManager shaderCache;
static std::unique_ptr<FluidRenderer> renderer;

void cursorPosition(GLFWwindow* window, double x, double y)
{
	mouse.x = static_cast<float>(x);
	mouse.y = static_cast<float>(y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouse.pressed = true;
		}
		else
		{
			mouse.pressed = false;
		}
	}
}

void switchDemo()
{
	++currentDemo;
	currentDemo = currentDemo % demoCount;
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int modifier)
{
	switch (key)
	{
	case GLFW_KEY_Q:
	{
		if (action == GLFW_RELEASE)
		{
			float currentRadius = renderer->getParticleRadius();
			if (currentRadius > particleIncrement)
			{
				renderer->setParticleRadius(currentRadius - particleIncrement);
			}
		}
	}
		break;
	case GLFW_KEY_W:
		if (action == GLFW_RELEASE) renderer->setParticleRadius(renderer->getParticleRadius() + particleIncrement);
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS) addingParticles = true;
		else if (action == GLFW_RELEASE) addingParticles = false;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_RELEASE) switchDemo();
		break;
	case GLFW_KEY_B:
		if (action == GLFW_RELEASE) renderer->toggleBlur();
		break;
	case GLFW_KEY_I:
		if (action == GLFW_RELEASE) renderer->toggleBlobInterpolation();
		break;
	case GLFW_KEY_P:
		if (action == GLFW_RELEASE) paused = !paused;
		break;
	case GLFW_KEY_L:
		if (action == GLFW_RELEASE) renderer->togglePositionRendering();
		break;
	case GLFW_KEY_G:
		if (action == GLFW_RELEASE) renderer->enableGrid(!renderer->isGridEnabled());
		break;
	default:
		break;
	}
}

void glfwErrorCallback(int error, const char* description)
{
	std::cout << "GLFW error: " << description << "\n";
}

void reshape(GLFWwindow* window, int width, int height)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.f, 1.f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 1.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1.f, 0)
		);
	glm::mat4 mvp = projection * view;
	shaderCache.setMVPMatrix(mvp);
}

int main(int argc, char** argv)
{
	if (!glfwInit())
		return -1;
	glfwSetErrorCallback(glfwErrorCallback);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto window = glfwCreateWindow(windowWidth, windowHeight, "FluidSim", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, reshape);
	glfwSetCursorPosCallback(window, cursorPosition);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW init failed. Exiting.\n";
		return -1;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Hotkeys: \n\t Q - decrease particle size"
		"\n\t W - increase particle size"
		"\n\t P - pause"
		"\n\t L - display particles"
		"\n\t B - toggle blur"
		"\n\t I - toggle interpolation + coloring shader"
		"\n\t Space - toggles between atomorph and the simulator\n";
				
	shaderCache.initialise();
	reshape(window, windowWidth, windowHeight);
	Drawing::init(&shaderCache, windowWidth, windowHeight);
	int scale = static_cast<int>(scaleFactor);
	auto fluid = Simulator::create(windowWidth / scale, windowHeight / scale);
	fluid->createParticles(particlesX, particlesY);
	fluid->step();
	float particleRadius = 8.f;
	renderer.reset(new FluidRenderer(windowWidth, windowHeight, scaleFactor, particleRadius, fluid.get(), &shaderCache));
	renderer->enableGrid(false);
	int width, height;
	size_t frameCount = 0;
	long long totalTime = 0;

	AtoDemo atoDemo(1, renderer.get());
	atoDemo.pause();

	while (!glfwWindowShouldClose(window))
	{

		auto start = std::chrono::high_resolution_clock::now();

		glfwGetFramebufferSize(window, &width, &height);
		auto ratio = width / (float)height;
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		switch (currentDemo)
		{
		case AtoMorph:
		{
						 if (atoDemo.paused)
						 {
							 renderer->setColorCutting(false);
							 renderer->setParticleRadius(4.f);
							 atoDemo.resume();
						 }
						 atoDemo.update();
						 atoDemo.drawScene();
		}
		break;
		case Fluid:
		{
					  if (!atoDemo.paused)
					  {
						  renderer->setColorCutting(true);
						  renderer->setParticleRadius(8.f);
						  atoDemo.pause();
					  }
					  fluid->setDrag(mouse.pressed);
					  fluid->setMovePos(mouse.x / scaleFactor, mouse.y / scaleFactor);
					  if (addingParticles)
					  {
						  fluid->addParticle(Particle(mouse.x / scaleFactor,
							  mouse.y / scaleFactor, 1.0f, 1.0f));

					  }
					  if (!paused) fluid->step();
					  renderer->updatePositions();
					  renderer->render(renderer->getSimulatorVertices());
		}
			break;
		default:
			break;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);


		glfwSwapBuffers(window);
		glfwPollEvents();
		totalTime += duration.count();
		frameCount++;

	}
	glfwTerminate();
	std::cout << "Finished. Average frame time: " << totalTime / frameCount << " ms.\n";
	return 0;
}