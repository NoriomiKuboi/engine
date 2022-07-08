#include "Noise.h"

Noise::Noise()
{
	for (int i = 0;i < 512;i++)
	{
		numCode[i] = { 0 };
	}
}

float Noise::Perlin(float x, float y, float z)
{
	int xi = (int)floorf(x);
	int yi = (int)floorf(y);
	int zi = (int)floorf(z);
	float xf = x - xi;
	float yf = y - yi;
	float zf = z - zi;

	float aaa = Grad(RandomGet(xi, yi, zi), xf, yf, xf);
	float baa = Grad(RandomGet(xi + 1, yi, zi), xf - 1.0f, yf, xf);
	float aba = Grad(RandomGet(xi, yi + 1, zi), xf, yf - 1.0f, xf);
	float aab = Grad(RandomGet(xi, yi, zi + 1), xf, yf, xf - 1.0f);
	float abb = Grad(RandomGet(xi, yi + 1, zi + 1), xf, yf - 1.0f, xf - 1.0f);
	float bab = Grad(RandomGet(xi + 1, yi, zi + 1), xf - 1.0f, yf, xf - 1.0f);
	float bba = Grad(RandomGet(xi + 1, yi + 1, zi), xf - 1.0f, yf - 1.0f, xf);
	float bbb = Grad(RandomGet(xi + 1, yi + 1, zi + 1), xf - 1.0f, yf - 1.0f, xf - 1.0f);

	float u = Fade(xf);
	float v = Fade(yf);
	float w = Fade(zf);

	float x1 = Lerp(aaa, baa, u);
	float x2 = Lerp(aba, bba, u);
	float x3 = Lerp(x1, x2, u);
	float x4 = Lerp(aab, bab, v);
	float x5 = Lerp(abb, bbb, v);
	float x6 = Lerp(x4, x5, v);

	return (Lerp(x3, x6, w) + 1) / 2;
}

float Noise::Grad(int hash, float x, float y, float z)
{
	int value = hash & 0xF;
	switch (value)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
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

	memset(numCode, 0, sizeof(unsigned int) * _countof(numCode));

	static const int num = maxNum;
	unsigned int randomTable[maxNum];
	for (int i = 0; i < _countof(randomTable); ++i)
	{
		randomTable[i] = rand() % maxNum;
	}

	for (int i = 0; i < _countof(numCode); ++i)
	{
		numCode[i] = randomTable[i % num];
	}
}

int Noise::RandomGet(int x, int y,int z)
{
	x %= maxNum;
	y %= maxNum;
	z %= maxNum;

	//return numCode[numCode[numCode[x] + y] + z];
	return numCode[x + numCode[y] + numCode[z]];
}
