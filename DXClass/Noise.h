#pragma once
#include "Object3d.h"

#include <DirectXMath.h>

class Noise
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Noise();
	float Perlin(float x, float y);
	float Grad(int hash, float x, float y);
	float Fade(float t);
	float Lerp(float a, float b, float x);
	void CreateRandom(unsigned int seed);
	int RandomGet(int x, int y);
	float ValueGet(int x, int y);

public:
	static const int splitNum = 255;
	static const int maxNum = 256;
	static const int tableNum = maxNum * 2;
	int numCode[tableNum];
};