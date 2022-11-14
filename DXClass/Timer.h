#pragma once
#include "SafeDelete.h"
#include "Sprite.h"
#include <cassert>
#include <sstream>
#include <iomanip>

class Timer
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Timer();
	~Timer();

	void Init(); // èâä˙âª
	void Update(); // çXêV
	void Draw(); // ï`âÊ
	void Reset(); //0Ç…èâä˙âª

	Sprite* num0One = nullptr;
	Sprite* num1One = nullptr;
	Sprite* num2One = nullptr;
	Sprite* num3One = nullptr;
	Sprite* num0Ten = nullptr;
	Sprite* num1Ten = nullptr;
	Sprite* num2Ten = nullptr;
	Sprite* num3Ten = nullptr;
	Sprite* num4 = nullptr;
	Sprite* num5 = nullptr;
	Sprite* num6 = nullptr;
	Sprite* num7 = nullptr;
	Sprite* num8 = nullptr;
	Sprite* num9 = nullptr;

	int time;
	int secOne;
	int secTen;
	int count;
	const XMFLOAT2 onePos = { 1280.0f / 2.0f,70.0f };
	const XMFLOAT2 tenPos = { 1280 / 2.0f - 48.0f,70.0f };
};