#pragma once
#include "Camera.h"
#include "Input.h"
#include "XInputManager.h"

class CameraSub : public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	CameraSub(int window_width, int window_height, Input* input);
	void Update() override; // �X�V

private:
	Input* input; // ���̓N���X�̃|�C���^
	float scaleX = 1.0f; // �X�P�[�����Ox
	float scaleY = 1.0f; // �X�P�[�����Oy
	XMMATRIX matRot = DirectX::XMMatrixIdentity(); // ��]�s��
};