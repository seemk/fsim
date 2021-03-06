#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "MPMSimulation.hpp"
#include "Box2DSimulation.hpp"
#include "FluidRenderer.hpp"
#include "Drawing.hpp"
#include "ShaderManager.hpp"
#include <chrono>
#include <iostream>

struct Mouse
{
	float x;
	float y;
	bool pressed;
};

namespace
{
	Mouse mouse = { 0.0f, 0.0f, false };
	const int windowWidth = 1280;
	const int windowHeight = 720;
	const int particlesX = 100;
	const int particlesY = 50;
	const float particleIncrement = 2.0f;
	const int demoCount = 2;
	bool paused = false;

	bool addingParticles = false;
	float scaleFactor = 8.f;

	ShaderManager shaderCache;
	std::unique_ptr<FluidRenderer> renderer;
	std::unique_ptr<FluidSimulation> simulation;
}


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
	case GLFW_KEY_1:
		if (action == GLFW_RELEASE)
		{
			auto mpmSimulation = std::make_unique<MPMSimulation>(ScreenSize(windowWidth, windowHeight), scaleFactor);
			simulation = std::move(mpmSimulation);
			simulation->createParticles(particlesX, particlesY);
			renderer->setSimulation(simulation.get());
		}
		break;
	case GLFW_KEY_2:
		if (action == GLFW_RELEASE)
		{
			auto box2DSimulation = std::make_unique<Box2DSimulation>(ScreenSize(windowWidth, windowHeight));
			simulation = std::move(box2DSimulation);
			renderer->setSimulation(simulation.get());
		}
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
		"\n\t (1-2) - switch simulations\n\n";
	


	shaderCache.initialise();
	reshape(window, windowWidth, windowHeight);
	Drawing::init(&shaderCache, windowWidth, windowHeight);

	auto box2DSimulation = std::make_unique<Box2DSimulation>(ScreenSize(windowWidth, windowHeight));
	simulation = std::move(box2DSimulation);

	float particleRadius = 6.f;

	renderer.reset(new FluidRenderer(windowWidth, windowHeight, particleRadius, simulation.get(), &shaderCache));
	int width, height;
	size_t frameCount = 0;
	long long totalTime = 0;
	while (!glfwWindowShouldClose(window))
	{

		auto start = std::chrono::high_resolution_clock::now();
		glfwGetFramebufferSize(window, &width, &height);
		auto ratio = width / (float)height;
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		simulation->setDragging(mouse.pressed);
		auto mousePosition = glm::vec2{ mouse.x, mouse.y };
		simulation->setInputPosition(mousePosition);
		if (addingParticles)
		{
			simulation->addParticles(mousePosition, 16);
		}
		if (!paused) simulation->update(0.167f); // TODO: Remove the constant
		renderer->updatePositions();
		renderer->render();
		simulation->debugDraw();
		
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