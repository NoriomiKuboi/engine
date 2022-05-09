#pragma once

#include <DirectXMath.h>

class Camera
{
protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Camera(int window_width, int window_height);
	virtual ~Camera() = default;

	virtual void Update(); // 更新

	void UpdateViewMatrix(); // ビュー行列を更新

	void UpdateProjectionMatrix(); // 射影行列を更新
	
	// ビュー行列の取得
	inline const XMMATRIX& GetViewMatrix(){ 
		return matView;
	}

	// 射影行列の取得
	inline const XMMATRIX& GetProjectionMatrix(){ 
		return matProjection;
	}

	// ビュー射影行列の取得
	inline const XMMATRIX& GetViewProjectionMatrix(){ 
		return matViewProjection;
	}

	// ビルボード行列の取得
	inline const XMMATRIX& GetBillboardMatrix(){ 
		return matBillboard;
	}

	inline const XMFLOAT3& GetEye(){ 
		return eye;
	}

	inline void SetEye(XMFLOAT3 eye) {
		this->eye = eye; 
		viewDirty = true;
	}

	inline const XMFLOAT3& GetTarget() {
		return target;
	}

	// 注視点座標の設定
	inline void SetTarget(XMFLOAT3 target) {
		this->target = target; 
		viewDirty = true;
	}

	// 上方向ベクトルの取得
	inline const XMFLOAT3& GetUp() {
		return up;
	}

	// 上方向ベクトルの設定
	inline void SetUp(XMFLOAT3 up) {
		this->up = up; 
		viewDirty = true;
	}

	/// ベクトルによる視点移動
	void MoveEyeVector(const XMFLOAT3& move);
	void MoveEyeVector(const XMVECTOR& move);

	// ベクトルによる移動
	void MoveVector(const XMFLOAT3& move);
	void MoveVector(const XMVECTOR& move);

protected:
	XMMATRIX matView = DirectX::XMMatrixIdentity(); // ビュー行列
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity(); // ビルボード行列
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity(); // Y軸回りビルボード行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity(); // 射影行列
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity(); // ビュー射影行列
	bool viewDirty = false; // ビュー行列ダーティフラグ
	bool projectionDirty = false; // 射影行列ダーティフラグ
	XMFLOAT3 eye = { 0, 0, -20 }; // 視点座標
	XMFLOAT3 target = { 0, 0, 0 }; // 注視点座標
	XMFLOAT3 up = { 0, 1, 0 }; // 上方向ベクトル
	float aspectRatio = 1.0f; // アスペクト比
};