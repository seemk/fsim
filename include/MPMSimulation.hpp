#pragma once
/*
	Original by Grant Kot (https://github.com/kotsoft/)
*/

#include "Array2D.hpp"
#include "ScreenSize.hpp"
#include <random>
#include <memory>
#include "FluidSimulation.hpp"

struct MPMNode
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

	MPMNode()
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

struct MPMParticle
{

	MPMParticle(float x_, float y_, float u_, float v_)
	: x(x_)
	, y(y_)
	, u(u_)
	, v(v_)
	{

	}

	float x;
	float y;
	float u;
	float v;

	float dudx;
	float dudy;
	float dvdx;
	float dvdy;
	unsigned cx;
	unsigned cy;

	float px[3];
	float py[3];
	float gx[3];
	float gy[3];

	void initialiseWeights()
	{
		cx = static_cast<int>(x - .5f);
		cy = static_cast<int>(y - .5f);

		float cx_x = cx - x;
		float cy_y = cy - y;

		px[0] = .5f * cx_x * cx_x + 1.5f * cx_x + 1.125f;
		gx[0] = cx_x + 1.5f;
		cx_x++;
		px[1] = -cx_x * cx_x + .75f;
		gx[1] = -2 * cx_x;
		cx_x++;
		px[2] = .5f * cx_x * cx_x - 1.5f * cx_x + 1.125f;
		gx[2] = cx_x - 1.5f;

		py[0] = .5f * cy_y * cy_y + 1.5f * cy_y + 1.125f;
		gy[0] = cy_y + 1.5f;
		cy_y++;
		py[1] = -cy_y * cy_y + .75f;
		gy[1] = -2 * cy_y;
		cy_y++;
		py[2] = .5f * cy_y * cy_y - 1.5f * cy_y + 1.125f;
		gy[2] = cy_y - 1.5f;
	}
};

struct MPMMaterial
{

	MPMMaterial(float m_, float rd_, float k_, float v_, float d_, float g_)
	: m(m_)
	, rd(rd_)
	, k(k_)
	, v(v_)
	, d(d_)
	, g(g_)
	{

	}

	float m;
	float rd;
	float k;
	float v;
	float d;
	float g;
};


class MPMSimulation : public FluidSimulation
{
	typedef std::uniform_real_distribution<float> distribution_t;

public:

	MPMSimulation(ScreenSize screenSize, float scale);

	void update(float dt) override;
	void createParticles(size_t horizontalAmount, size_t verticalAmount) override;
	void addParticles(glm::vec2 location, size_t count) override;
	size_t getParticleCount() const override;
	void setDragging(bool drag) override;
	void setInputPosition(glm::vec2 position) override;
	std::vector<glm::vec2> getParticlePositions() const override;
	void debugDraw() const override;

	distribution_t distribution;
	std::mt19937 generator;

	Array2D<MPMNode>& getGrid() { return grid; }

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
	float	simulationScale = 1.0f;

	Array2D<MPMNode>			grid;
	std::vector<MPMNode*>		active;
	std::vector<MPMParticle>   particles;

	MPMMaterial mat;
};