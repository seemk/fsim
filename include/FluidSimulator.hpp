#pragma once

/*
	Original by Grant Kot (https://github.com/kotsoft/)
*/

#include "Array2D.hpp"
#include "Particle.hpp"
#include "Material.hpp"
#include <glm/glm.hpp>
#include <random>
#include <memory>

struct Node
{
	void clear();

	float m;
	float d;
	float gx;
	float gy;
	float u;
	float v;
	float ax;
	float ay;
	bool active;

	Node()
		: m(0.0f)
		, gx(0.0f)
		, gy(0.0f)
		, u(0.0f)
		, v(0.0f)
		, ax(0.0f)
		, ay(0.0f)
		, active(false)
	{ }
};

class Simulator
{
	typedef std::uniform_real_distribution<float> Distribution;

public:

	static std::unique_ptr<Simulator> create(int windowWidth, int windowHeight);

	Simulator(int gridSizeX, int gridSizeY);
	void createParticles(size_t countX, size_t countY);
	void addParticle(const Particle& p);
	void step();
	void setDrag(bool drag);
	void setMovePos(float x, float y);

	std::vector<Particle>& getParticles();
	std::vector<glm::vec2> getParticlePositions() const;
	Array2D<Node>& getGrid() { return grid; }

	std::random_device rd;
	Distribution distribution;
	std::mt19937 generator;

	float genRand() { return distribution(generator); }
	size_t particleCount() const { return particles.size(); }

private:

	int     gsizeX;
	int     gsizeY;
	bool	pressed;
	bool	pressedPrev;
	float	mx;
	float	my;
	float	mxPrev;
	float	myPrev;

	Array2D<Node>			grid;
	std::vector<Node*>		active;
	std::vector<Particle>   particles;

	Material mat;
};