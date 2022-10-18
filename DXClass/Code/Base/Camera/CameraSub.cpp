#include "CameraSub.h"
#include <cassert>

using namespace DirectX;

CameraSub::CameraSub(int windWidth, int winHeight, Input* input) : Camera(windWidth, winHeight)
{
	assert(input);

	this->input = input;
	// 画面サイズに対する相対的なスケールに調整
	scaleX = 0.3f / (float)windWidth;
	scaleY = 0.3f / (float)winHeight;
}

void CameraSub::Update()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	// ゲームパッドの入力を取得
	XInputManager* Xinput = XInputManager::GetInstance();
	float RSticlRot = Xinput->GetPadRStickAngle() - 90;
	float LSticlRot = Xinput->GetPadLStickAngle();
	float rotR = XMConvertToRadians(RSticlRot);
	float rotL = XMConvertToRadians(LSticlRot);

	// 右スティックでカメラを回転
	//if (Xinput->RightStickX(true) || Xinput->RightStickX(false) || Xinput->RightStickY(true) || Xinput->RightStickY(false))
	//{
	//	//input->PushMouseLeft()
	//	//float dy = mouseMove.lY * scaleY;
	//	//float dx = mouseMove.lX * scaleX;
	//	float dy = sinf(rotL) * scaleY;
	//	float dx = cosf(rotR) * scaleX;
	//
	//	angleX = -dx * XM_PI;
	//	angleY = -dy * XM_PI;
	//	dirty = true;
	//}

	// 左スティックでカメラを移動
	if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false) || Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		//input->PushMouseMiddle()
		//float dx = mouseMove.lX / 100.0f;
		//float dy = mouseMove.lY / 100.0f;
		float dx = cosf(rotL) * 0.3f;
		float dy = sinf(rotL) * 0.3f;
	
		XMVECTOR move = { dx, dy, 0, 0 };
		move = XMVector3Transform(move, matRot);
	
		MoveVector(move);
		dirty = true;
	}
	
	// ホイールで距離を変更
	//if (mouseMove.lZ != 0)
	//{
	//	distance -= mouseMove.lZ / 100.0f;
	//	distance = max(distance, 1.0f);
	//	dirty = true;
	//}

	if (dirty || viewDirty)
	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);

		// 累積の回転行列を合成
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 注視点からずらした位置に視点座標を決定
		const XMFLOAT3& target = GetTarget();
		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	Camera::Update();
}
