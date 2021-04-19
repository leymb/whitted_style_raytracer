#pragma once
struct rgbColor
{
	rgbColor(int aR = 0, int aG = 0, int aB = 0);
	rgbColor(vec3 aVec3);
	
	int r = 0;
	int g = 0;
	int b = 0;
};

