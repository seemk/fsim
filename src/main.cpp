#include <GLFW/glfw3.h>
#include "fluidmodel.hpp"
#include <memory>
#include <chrono>
#include <iostream>

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

	auto fluid = fsim::Simulator::create(windowWidth, windowHeight);
	fluid->createParticles(150, 150);
	fluid->step();

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1.0f);
	float ratio = 0.0f;
	int width, height;

	size_t frameCount = 0;
	long long totalTime = 0;
    while (!glfwWindowShouldClose(window))
    {
		
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
		glBegin(GL_POINTS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		auto start = std::chrono::high_resolution_clock::now();
		fluid->step();
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		auto particle_count = fluid->particleCount();
		auto particles = fluid->getParticles();
		for (size_t i = 0; i < particle_count; ++i) {
			const auto& particle = particles[i];
			float x1 = particle.x * 4.f;
			float y1 = height - particle.y * 4.f;
			glVertex2f(x1, y1);
		}

		glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
		totalTime += duration.count();
		frameCount++;
    }
    glfwTerminate();
	std::cout << "Finished. Average step time: " << totalTime / frameCount << " ms.\n";
    return 0;
}