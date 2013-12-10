#include "AtoDemo.hpp"
#include "Vertex.hpp"
#include "Drawing.hpp"
#include "stb_image.h"
#include <memory>

AtoDemo::AtoDemo(size_t threadCount)
	: paused(false)
	, threadCount(threadCount)
	, selectedThreadId(-1)
	, randomEngine(std::random_device()())
	, distribution(1, std::numeric_limits<unsigned>::max())
	, threads(threadCount)
{
	scene.init(40000, 3);
	fillScene(0, "data/cat_1.png");
	fillScene(1, "data/cat_2.png");
	fillScene(2, "data/cat_3.png");
	scene.elect_atoms();
	scene.renew_splines();

	for (auto& thread : threads)
	{
		thread.init(&scene, distribution(randomEngine), 100, 3.0);
		thread.start();
	}

	scene_cost = scene.get_cost();
}

void AtoDemo::fillScene(size_t frame, const char* imagePath)
{
	auto generator = std::mt19937(std::random_device()());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	auto rand = std::bind(distribution, std::ref(generator));
	int imageWidth = 0, imageHeight = 0, comp = 0;

	auto imageBuffer = std::unique_ptr<stbi_uc[]>(stbi_load(imagePath, &imageWidth, &imageHeight, &comp, 0));

	float fImageWidth = static_cast<float>(imageWidth);
	float fImageHeight = static_cast<float>(imageHeight);

	unsigned char r, g, b, a;
	float px = 0.0f, py = 0.0f;
	for (int x = 0; x < imageWidth; ++x)
	{
		for (int y = 0; y < imageHeight; ++y)
		{
			int index = (imageWidth * y + x) * 4;
			r = imageBuffer[index];
			g = imageBuffer[index + 1];
			b = imageBuffer[index + 2];
			a = imageBuffer[index + 3];

			if (a == 0) continue;

			px = (1.0f * x + rand()) / fImageWidth;
			py = (1.0f * y + rand()) / fImageHeight;

			scene.push_atom(frame, am_create_atom(px, py, r, g, b, a));
		}
	}
}

void AtoDemo::drawScene()
{

	std::vector<Vertex> vertices;
	vertices.reserve(10000);
	morph_time++;
	float t = (morph_time % slowness) / static_cast<float>(slowness);
	if (morph_time == slowness + 1) morph_time = 0;

	size_t atoms = scene.atom_count();
	for (size_t i = 0; i<atoms; ++i) {
		float x, y;
		float px, py;
		scene.get_xy(i, t, &px, &py);
		if (px > 1.0) px = 1.0;
		if (px < 0.0) px = 0.0;
		if (py > 1.0) py = 1.0;
		if (py < 0.0) py = 0.0;
		x = px*morphWidth;
		y = py*morphHeight;
		unsigned char r, g, b, a;
		scene.get_rgba(i, t, &r, &g, &b, &a);
		vertices.emplace_back(x * 3, y * 2, r, g, b, 10);
	}

	//Drawing::drawBlobs(vertices, 5.f, 12);
}

void AtoDemo::update()
{
	if (morph_time%slowness == 0) {
		size_t best_thread = 0;
		float best_cost = 0.0;
		bool   found = false;

		for (size_t i = 0; i<threadCount; ++i) {
			if (!threads[i].is_paused()) {
				threads[i].pause();
				float cost = threads[i].get_cost();
				if (!found) {
					best_thread = i;
					best_cost = cost;
					found = true;
				}
				else if (cost < best_cost){
					best_cost = cost;
					best_thread = i;
				}
			}
		}

		if (found && scene_cost > best_cost) {
			threads[best_thread].fetch_scene(&scene);
			scene.renew_splines();
			scene_cost = best_cost;
			selectedThreadId = best_thread;
			for (size_t i = 0; i<threadCount; ++i) {
				threads[i].init(&scene, distribution(randomEngine), 1000, 3.0);
			}
		}

		for (size_t i = 0; i<threadCount; ++i) {
			if (threads[i].is_paused()) {
				threads[i].resume();
			}
		}
	}
}

void AtoDemo::pause()
{
	for (auto& thread : threads)
	{
		thread.pause();
	}
	paused = true;
}
void AtoDemo::resume()
{
	for (auto& thread : threads)
	{
		thread.resume();
	}
	paused = false;
}