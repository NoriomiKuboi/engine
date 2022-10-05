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

	virtual void Update(); // �X�V

	void UpdateViewMatrix(); // �r���[�s����X�V

	void UpdateProjectionMatrix(); // �ˉe�s����X�V
	
	// �r���[�s��̎擾
	inline const XMMATRIX& GetViewMatrix(){ 
		return matView;
	}

	// �ˉe�s��̎擾
	inline const XMMATRIX& GetProjectionMatrix(){ 
		return matProjection;
	}

	// �r���[�ˉe�s��̎擾
	inline const XMMATRIX& GetViewProjectionMatrix(){ 
		return matViewProjection;
	}

	// �r���{�[�h�s��̎擾
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

	// �����_���W�̐ݒ�
	inline void SetTarget(XMFLOAT3 target) {
		this->target = target; 
		viewDirty = true;
	}

	// ������x�N�g���̎擾
	inline const XMFLOAT3& GetUp() {
		return up;
	}

	// ������x�N�g���̐ݒ�
	inline void SetUp(XMFLOAT3 up) {
		this->up = up; 
		viewDirty = true;
	}

	/// �x�N�g���ɂ�鎋�_�ړ�
	void MoveEyeVector(const XMFLOAT3& move);
	void MoveEyeVector(const XMVECTOR& move);

	// �x�N�g���ɂ��ړ�
	void MoveVector(const XMFLOAT3& move);
	void MoveVector(const XMVECTOR& move);

protected:
	XMMATRIX matView = DirectX::XMMatrixIdentity(); // �r���[�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity(); // �r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity(); // Y�����r���{�[�h�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity(); // �ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity(); // �r���[�ˉe�s��
	bool viewDirty = false; // �r���[�s��_�[�e�B�t���O
	bool projectionDirty = false; // �ˉe�s��_�[�e�B�t���O
	XMFLOAT3 eye = { 0, 0, -20 }; // ���_���W
	XMFLOAT3 target = { 0, 0, 0 }; // �����_���W
	XMFLOAT3 up = { 0, 1, 0 }; // ������x�N�g��
	float aspectRatio = 1.0f; // �A�X�y�N�g��
};