#pragma once
#include "Camera.h"
#include "Input.h"
#include "XInputManager.h"

class CameraSub : public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	CameraSub(int window_width, int window_height, Input* input);
	void Update() override; // 更新

private:
	Input* input; // 入力クラスのポインタ
	float scaleX = 1.0f; // スケーリングx
	float scaleY = 1.0f; // スケーリングy
	XMMATRIX matRot = DirectX::XMMatrixIdentity(); // 回転行列
};