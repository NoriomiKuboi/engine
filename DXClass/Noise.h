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
	float Perlin(float x, float y, float z);
	float Grad(int hash, float x, float y, float z);
	float Fade(float t);
	float Lerp(float a, float b, float x);
};