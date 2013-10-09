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

int main(int argc, char** argv)
{
    if (!glfwInit())
        return -1;

	const int windowWidth = 1280;
	const int windowHeight = 720;

    auto window = glfwCreateWindow(windowWidth, windowHeight, "FluidSim", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, cursorPosition);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW init failed. Exiting.\n";
		return -1;
	}

	auto fluid = Simulator::create(windowWidth, windowHeight);
	fluid->createParticles(25, 25);
	fluid->step();

	float ratio = 0.0f;
	int width, height;

	size_t frameCount = 0;
	long long totalTime = 0;
    while (!glfwWindowShouldClose(window))
    {
		auto start = std::chrono::high_resolution_clock::now();
		fluid->setDrag(mouse.pressed);
		fluid->setMovePos(mouse.x / 8.f, mouse.y / 8.f);
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
		glColor3f(1.0f, 0.0f, 0.0f);

		auto particle_count = fluid->particleCount();
		auto particles = fluid->getParticles();
		for (size_t i = 0; i < particle_count; ++i)
		{
			const auto& particle = particles[i];
			float x1 = particle.x * 8.f;
			float y1 = height - particle.y * 8.f;

			DrawingPrimitives::drawCircle(glm::vec2(x1, y1), 10.0f);
		}

		auto grid = fluid->getGrid();
		float w = width / static_cast<float>(grid.rows());
		float h = height / static_cast<float>(grid.cols());
		glColor3f(0.5f, 0.5f, 0.5f);
		for (size_t y = 0; y < grid.cols(); y++)
		{
			for (size_t x = 0; x < grid.rows(); x++)
			{
				glm::vec2 p1(x * w, y * h);
				glm::vec2 p2(p1.x + w, p1.y + h);
				DrawingPrimitives::drawRectangle(p1, p2);
			}
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