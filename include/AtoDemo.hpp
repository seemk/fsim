#pragma once

#include "atomorph/atomorph.h"
#include <random>

class AtoDemo
{
public:

	AtoDemo(size_t threadCount);

	void update();
	void drawScene();
	void pause();
	void resume();

	bool paused;

private:

	size_t morph_time = 0;
	size_t slowness = 100;
	int morphWidth = 256;
	int morphHeight = 256;
	size_t threadCount;
	float scene_cost;
	int selectedThreadId;

	std::mt19937 randomEngine;
	std::uniform_int_distribution<unsigned> distribution;

	void fillScene(size_t frame, const char* imagePath);
	
	std::vector<AM_THREAD> threads;
	AM_SCENE scene;
};