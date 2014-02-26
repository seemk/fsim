#pragma once

struct ScreenSize
{

	ScreenSize(int screenWidth, int screenHeight)
	: width(static_cast<float>(screenWidth))
	, height(static_cast<float>(screenHeight))
	, iWidth(screenWidth)
	, iHeight(screenHeight)
	{

	}

	float width;
	float height;
	int iWidth;
	int iHeight;

};