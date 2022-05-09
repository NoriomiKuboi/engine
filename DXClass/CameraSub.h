#pragma once
#include "Camera.h"
#include "Input.h"

class CameraSub : public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	CameraSub(int window_width, int window_height, Input* input);
	void Update() override; // �X�V

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

private:
	Input* input; // ���̓N���X�̃|�C���^
	float distance = 20; // �J���������_�܂ł̋���
	float scaleX = 1.0f; // �X�P�[�����Ox
	float scaleY = 1.0f; // �X�P�[�����Oy
	XMMATRIX matRot = DirectX::XMMatrixIdentity(); // ��]�s��
};