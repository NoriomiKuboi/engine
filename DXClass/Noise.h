#pragma once
#include <DirectXMath.h>

class Noise
{
public:
	float Perlin(float x, float y, float z);
	float Grad(int hash, float x, float y, float z);
	float Fade(float t);
	float Lerp(float a, float b, float x);
};