#include "Noise.h"

Noise::Noise()
{
	for (int i = 0;i < 512;i++)
	{
		numCode[i] = { 0 };
	}
}

float Noise::Perlin(float x, float y)
{
	//int xi = (int)x & 255;
	//int yi = (int)y & 255;
	//float xf = x - floorf(xi);
	//float yf = y - floorf(yi);
	//float u = Fade(xf);
	//float v = Fade(yf);
	int xi = (int)x / splitNum;
	int yi = (int)y / splitNum;
	float xf = x - xi;
	float yf = y - yi;
	float u = (x - splitNum * xi) / splitNum;
	float v = (y - splitNum * yi) / splitNum;
	Fade(u);
	Fade(v);

	// Œù”z‚ÌŒvŽZ
	float aa = Grad(RandomGet(xi    , yi    ), xf       , yf);
	float ba = Grad(RandomGet(xi + 1, yi    ), xf - 1.0f, yf);
	float ab = Grad(RandomGet(xi    , yi + 1), xf       , yf - 1.0f);
	float bb = Grad(RandomGet(xi + 1, yi + 1), xf - 1.0f, yf - 1.0f);

	// •âŠÔ
	float x1 = Lerp(aa, ba, u);
	float x2 = Lerp(ab, bb, u);
	float x3 = Lerp(x1, x2, v);

	return (x3 + 1.0f) / 2.0f;
}

float Noise::Grad(int hash, float x, float y)
{
	int value = hash % 0x4;
	switch (value)
	{
	//case 0x0: return x;
	//case 0x1: return -x;
	//case 0x2: return -y;
	//case 0x3: return y;
	case 0x0: return x + y;
	case 0x1: return -x + y;
	case 0x2: return x - y;
	case 0x3: return -x - y;
	default: return 0;
	}
}

float Noise::Fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Noise::Lerp(float a, float b, float x)
{
	return a + x * (b - a);
}	

void Noise::CreateRandom(unsigned int seed)
{
	srand(seed);

	static const int num = maxNum;
	int randomTable[maxNum];
	for (int i = 0; i < _countof(randomTable); i++)
	{
		randomTable[i] = rand() % maxNum;
	}

	for (int i = 0; i < _countof(numCode); i++)
	{
		numCode[i] = randomTable[i % num];
	}
}

int Noise::RandomGet(int x, int y)
{
	x %= maxNum;
	y %= maxNum;

	return numCode[numCode[x] + y];
	//return numCode[x + numCode[y]];
}