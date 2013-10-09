#include <cmath>
#include <algorithm>
#include "fluidsimulator.hpp"
#include <iostream>

void Node::clear()
{
	m = d = gx = gy = u = v = ax = ay = 0.0;
	active = false;
}

std::unique_ptr<Simulator> Simulator::create(int windowWidth, int windowHeight)
{
	int sim_w = windowWidth;
	int sim_h = windowHeight;
	auto fluid = std::make_unique<Simulator>(sim_w / 8, sim_h / 8);
	return fluid;
}

Simulator::Simulator(int gridSizeX, int gridSizeY)
	: gsizeX(gridSizeX)
	, gsizeY(gridSizeY)
	, pressed(false)
	, pressedPrev(false)
	, mx(0.0f)
	, my(0.0f)
	, mxPrev(0.0f)
	, myPrev(0.0f)
	, mat(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)
	, grid(gsizeX, gsizeY)
	, generator(rd())
	, distribution(0.0f, 0.01f)
{

}

void Simulator::createParticles(size_t countX, size_t countY)
{
	auto particleCount = countX * countY;
	particles.reserve(particleCount);
	for (size_t y = 0; y < countY; ++y)
	{
		for (size_t x = 0; x < countX; ++x)
		{
			auto particle = Particle(x + 4.f, y + 4.f, 1.0f, 0.0f);
			particles.push_back(particle);
		}
	}
	std::cout << "Created " << particles.size() << " particles.\n";
}

void Simulator::step()
{
	bool drag = false;
	
	float mdx = 0.0f, mdy = 0.0f;

	if (pressed && pressedPrev)
	{
		drag = true;
		mdx = (mx - mxPrev);
		mdy = (my - myPrev);
	}


	pressedPrev = pressed;
	mxPrev = mx;
	myPrev = my;

	for (auto& n : active)
	{
		n->clear();
	}
	active.clear();


	float phi;
	float x, y;
	float fx = 0.0f, fy = 0.0f;

	for (auto& p : particles) {

		p.cx = static_cast<int>(p.x - 0.5f);
		p.cy = static_cast<int>(p.y - 0.5f);

		x = p.cx - p.x;

		p.px[0] = (0.5f * x * x + 1.5f * x + 1.125f);
		p.gx[0] = (x + 1.5f);
		x += 1.0f;
		p.px[1] = (-x * x + 0.75f);
		p.gx[1] = (-2.0f * x);
		x += 1.0;
		p.px[2] = (0.5f * x * x - 1.5f * x + 1.125f);
		p.gx[2] = (x - 1.5f);

		y = p.cy - p.y;

		p.py[0] = (0.5f * y * y + 1.5f * y + 1.125f);
		p.gy[0] = (y + 1.5f);
		y += 1.0;
		p.py[1] = (-y * y + 0.75f);
		p.gy[1] = (-2.0f * y);
		y += 1.0;
		p.py[2] = (0.5f * y * y - 1.5f * y + 1.125f);
		p.gy[2] = (y - 1.5f);

		for (unsigned i = 0; i < 3; ++i) {
			for (unsigned j = 0; j < 3; ++j) {
				auto& n = grid(p.cx + i, p.cy + j);
				if (!n.active)
				{
					active.push_back(&n);
					n.active = true;
				}

				phi = p.px[i] * p.py[j];
				n.m += phi * mat.m;
				n.d += phi;
				n.gx += p.gx[i] * p.py[j];
				n.gy += p.px[i] * p.gy[j];
			}
		}

	}

	unsigned cx, cy;
	unsigned cxi, cyi;

	float pdx, pdy;
	float C20, C02, C30, C03;
	float C21, C31, C12, C13, C11;

	float csum1, csum2;

	float u, u2, u3;
	float v, v2, v3;

	float density;
	float pressure;

	float vx = 0.0f, vy = 0.0f;
	float weight = 0.0f;

	for (const auto& p : particles)
	{
		cx = static_cast<int>(p.x);
		cy = static_cast<int>(p.y);
		cxi = cx + 1;
		cyi = cy + 1;

		const auto& n01 = grid(cx, cy);
		const auto& n02 = grid(cx, cyi);
		const auto& n11 = grid(cxi, cy);
		const auto& n12 = grid(cxi, cyi);

		pdx = n11.d - n01.d;
		pdy = n02.d - n01.d;
		C20 = 3.0f * pdx - n11.gx - 2.0f * n01.gx;
		C02 = 3.0f * pdy - n02.gy - 2.0f * n01.gy;
		C30 = -2.0f * pdx + n11.gx + n01.gx;
		C03 = -2.0f * pdy + n02.gy + n01.gy;

		csum1 = n01.d + n01.gy + C02 + C03;
		csum2 = n01.d + n01.gx + C20 + C30;

		C21 = 3.0f * n12.d - 2.0f * n02.gx - n12.gx -
			3.0f * csum1 - C20;
		C31 = -2.0f * n12.d + n02.gx + n12.gx +
			2.0f * csum1 - C30;
		C12 = 3.0f * n12.d - 2.0f * n11.gy - n12.gy -
			3.0f * csum2 - C02;
		C13 = -2.0f * n12.d + n11.gy + n12.gy +
			2.0f * csum2 - C03;
		C11 = n02.gx - C13 - C12 - n01.gx;

		u = p.x - cx;
		u2 = u * u;
		u3 = u * u2;
		v = p.y - cy;
		v2 = v * v;
		v3 = v * v2;

		density = n01.d + n01.gx * u +
			n01.gy * v + C20 * u2 +
			C02 * v2 + C30 * u3 +
			C03 * v3 + C21 * u2 * v +
			C31 * u3 * v + C12 * u * v2 +
			C13 * u * v3 + C11 * u * v;

		pressure = density - 1.0f;
		if (pressure > 2.0f) {
			pressure = 2.0f;
		}

		fx = fy = 0.0f;

		if (p.x < 4.0f) {
			fx += mat.m * (4.0f - p.x);
		}
		else if (p.x > gsizeX - 5) {
			fx += mat.m * (gsizeX - 5 - p.x);
		}

		if (p.y < 4.0f) {
			fy += mat.m * (4.0f - p.y);
		}
		else if (p.y > gsizeY - 5) {
			fy += mat.m * (gsizeY - 5 - p.y);
		}

		if (drag)
		{
			vx = fabs(p.x - mx);
			vy = fabs(p.y - my);
			if (vx < 10.0f && vy < 10.0f)
			{
				weight = mat.m * (1.0f - vx * 0.1f) *
					(1.0f - vy * 0.1f);
				fx += weight * (mdx - p.u);
				fy += weight * (mdy - p.v);
			}
		}


		for (unsigned i = 0; i < 3; ++i) {
			for (unsigned j = 0; j < 3; ++j) {
				auto& n = grid(p.cx + i, p.cy + j);
				phi = p.px[i] * p.py[j];
				n.ax += -((p.gx[i] * p.py[j]) * pressure) + fx * phi;
				n.ay += -((p.px[i] * p.gy[j]) * pressure) + fy * phi;
			}
		}
	}

	for (auto& n : active)
	{
		if (n->m > 0.0) {
			n->ax /= n->m;
			n->ay /= n->m;
			n->ay += 0.03f;
		}
	}

	float mu, mv;

	for (auto& p : particles) {
		for (unsigned i = 0; i < 3; ++i) {
			for (unsigned j = 0; j < 3; ++j) {
				auto& n = grid(p.cx + i, p.cy + j);
				phi = p.px[i] * p.py[j];
				p.u += phi * n.ax;
				p.v += phi * n.ay;
			}
		}
		mu = mat.m * p.u;
		mv = mat.m * p.v;
		for (unsigned i = 0; i < 3; ++i) {
			for (unsigned j = 0; j < 3; ++j) {
				auto& n = grid(p.cx + i, p.cy + j);
				phi = p.px[i] * p.py[j];
				n.u += phi * mu;
				n.v += phi * mv;
			}
		}
	}

	for (auto& n : active)
	{
		if (n->m > 0.0f) {
			n->u /= n->m;
			n->v /= n->m;
		}
	}

	float gu, gv;

	for (auto& p : particles) {
		gu = 0.0f;
		gv = 0.0f;
		for (unsigned i = 0; i < 3; ++i) {
			for (unsigned j = 0; j < 3; ++j) {
				const auto& n = grid(p.cx + i, p.cy + j);
				phi = p.px[i] * p.py[j];
				gu += phi * n.u;
				gv += phi * n.v;
			}
		}

		p.x += gu;
		p.y += gv;
		p.u += gu - p.u;
		p.v += gv - p.v;
		if (p.x < 1.0f) {
			p.x = 1.0f + genRand();
			p.u = 0.0f;
		}
		else if (p.x > gsizeX - 2) {
			p.x = gsizeX - 2 - genRand();
			p.u = 0.0f;
		}
		if (p.y < 1.0f) {
			p.y = 1.0f + genRand();
			p.v = 0.0f;
		}
		else if (p.y > gsizeY - 2) {
			p.y = gsizeY - 2 - genRand();
			p.v = 0.0f;
		}
	}
}

std::vector<Particle>& Simulator::getParticles()
{
	return particles;
}

void Simulator::setDrag(bool drag)
{
	pressed = drag;
}

void Simulator::setMovePos(float x, float y)
{
	mx = x;
	my = y;
}