#include "CameraSub.h"
#include <cassert>

using namespace DirectX;

CameraSub::CameraSub(int windWidth, int winHeight, Input* input) : Camera(windWidth, winHeight)
{
	assert(input);

	this->input = input;
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 0.3f / (float)windWidth;
	scaleY = 0.3f / (float)winHeight;
}

void CameraSub::Update()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	// �}�E�X�̍��{�^����������Ă�����J��������]
	if (input->PushMouseLeft())
	{
		float dy = mouseMove.lX * scaleY;
		float dx = mouseMove.lY * scaleX;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;
		dirty = true;
	}

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�
	if (input->PushMouseMiddle())
	{
		float dx = mouseMove.lX / 100.0f;
		float dy = mouseMove.lY / 100.0f;

		XMVECTOR move = { -dx, +dy, 0, 0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	// �z�C�[���ŋ�����ύX
	if (mouseMove.lZ != 0)
	{
		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty || viewDirty)
	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);

		// �ݐς̉�]�s�������
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		const XMFLOAT3& target = GetTarget();
		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	Camera::Update();
}
