#pragma once

struct Material
{

	Material(float m_, float rd_, float k_, float v_, float d_, float g_)
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