#pragma once

struct Particle
{

	Particle(float x_, float y_, float u_, float v_)
		: x(x_)
		, y(y_)
		, u(u_)
		, v(v_)
		, r(255)
		, g(255)
		, b(255)
		
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

	unsigned char r;
	unsigned char g;
	unsigned char b;

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
