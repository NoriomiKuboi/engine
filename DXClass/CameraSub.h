#pragma once
#include "Camera.h"
#include "Input.h"

class CameraSub : public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	CameraSub(int window_width, int window_height, Input* input);
	void Update() override; // 更新

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

private:
	Input* input; // 入力クラスのポインタ
	float distance = 20; // カメラ注視点までの距離
	float scaleX = 1.0f; // スケーリングx
	float scaleY = 1.0f; // スケーリングy
	XMMATRIX matRot = DirectX::XMMatrixIdentity(); // 回転行列
};