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
	Noise(); // コンストラクタ
	float Perlin(float x, float y); // ノイズ計算
	void CreateRandom(unsigned int seed); // 乱数テーブルの生成

private:
	// 変数
	static const int splitNum = 10; // 分割数
	static const int maxNum = 256; // 最大数
	static const int tableNum = maxNum * 2; // 乱数テーブルの最大値
	int numCode[tableNum];

	// 関数
	float Grad(int hash, float x, float y); // グラディエント関数(勾配を計算)
	float Fade(float t); // フェード関数
	float Lerp(float a, float b, float x); // 線形補間
	int RandomGet(int x, int y); // 乱数の取得
};