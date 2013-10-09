#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "fluidsimulator.hpp"
#include <memory>
#include <chrono>
#include <iostream>
#include "DrawingPrimitives.hpp"
#include <glm\glm.hpp>

struct Mouse
{
	float x;
	float y;
	bool pressed;
};

Mouse mouse = { 0.0f, 0.0f, false };
const int windowWidth	= 1280;
const int windowHeight	= 720;
const int particlesX	= 25;
const int particlesY	= 25;

bool addingParticles	= false;
float scaleFactor		= 8.f;
float particleRadius	= 10.f;

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
		if (particleRadius >= 2.0f) particleRadius -= 1.0f;
		break;
	case GLFW_KEY_W:
		particleRadius += 1.0f;
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS) addingParticles = true;
		else if (action == GLFW_RELEASE) addingParticles = false;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	if (!glfwInit())
		return -1;

	auto window = glfwCreateWindow(windowWidth, windowHeight, "FluidSim", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, cursorPosition);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW init failed. Exiting.\n";
		return -1;
	}

	auto fluid = Simulator::create(windowWidth, windowHeight);
	fluid->createParticles(particlesX, particlesY);
	fluid->step();

	float ratio = 0.0f;
	int width, height;

	size_t frameCount = 0;
	long long totalTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		auto start = std::chrono::high_resolution_clock::now();
		fluid->setDrag(mouse.pressed);
		fluid->setMovePos(mouse.x / scaleFactor, mouse.y / scaleFactor);
		if (addingParticles) fluid->addParticle(Particle(mouse.x / scaleFactor, mouse.y / scaleFactor, 1.0f, 1.0f));
		fluid->step();
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / static_cast<float>(height);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, 0, height, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(0.15f, 0.15f, 0.15f);
		auto grid = fluid->getGrid();
		float w = width / static_cast<float>(grid.rows());
		float h = height / static_cast<float>(grid.cols());

		for (size_t y = 0; y < grid.cols(); y++)
		{
			for (size_t x = 0; x < grid.rows(); x++)
			{
				glm::vec2 p1(x * w, y * h);
				glm::vec2 p2(p1.x + w, p1.y + h);
				DrawingPrimitives::drawRectangle(p1, p2);
			}
		}

		glColor3f(1.0f, 0.0f, 0.0f);
		auto particle_count = fluid->particleCount();
		auto particles = fluid->getParticles();
		for (size_t i = 0; i < particle_count; ++i)
		{
			const auto& particle = particles[i];
			float x1 = particle.x * scaleFactor;
			float y1 = height - particle.y * scaleFactor;

			DrawingPrimitives::drawCircle(glm::vec2(x1, y1), particleRadius, 16);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		totalTime += duration.count();
		frameCount++;
	}
	glfwTerminate();
	std::cout << "Finished. Average step time: " << totalTime / frameCount << " ms.\n";
	return 0;
}