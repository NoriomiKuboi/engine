#include "Noise.h"

float Noise::Perlin(float x, float y, float z)
{
	
}

float Noise::Grad(int hash, float x, float y, float z)
{
	
}

float Noise::Fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Noise::Lerp(float a, float b, float x)
{
	return a + x * (b - a);
}
