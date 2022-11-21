#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "CollisionInfo.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Quaternion.h"

class BaseCollider;

class Object3d
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// �p�C�v���C���Z�b�g
	struct PipelineSet
	{
		ComPtr<ID3D12RootSignature> rootsignature; // ���[�g�V�O�l�`��
		ComPtr<ID3D12PipelineState> pipelinestate; // �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	};

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		//XMMATRIX mat;	// �R�c�ϊ��s��
		XMFLOAT4 color;
		XMMATRIX viewproj;// �r���[�v���W�F�N�V�����s��
		XMMATRIX world;// ���[���h�s��
		XMFLOAT3 cameraPos;// �J�������W(���[���h���W)
	};

public:
	static void StaticInit(ID3D12Device* device, Camera* camera = nullptr); // ������
	static void CreateGraphicsPipeline(); // �O���t�B�b�N�p�C�v���C���̐���

	// �J�����̃Z�b�g
	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	static void BeforeDraw(ID3D12GraphicsCommandList* cmdList); // �`��O����
	static void AfterDraw(); // �`��㏈��
	static std::unique_ptr<Object3d> Create(Model* model = nullptr); // 3D�I�u�W�F�N�g����
	static void SetLight(Light* light) { Object3d::light = light; } //���C�g�̃Z�b�g

private:
	static ID3D12Device* dev;
	static ID3D12GraphicsCommandList* cmdList;
	static PipelineSet pipelineSet;
	static Camera* camera;
	static Light* light;

public:
	Object3d() = default; // �R���X�g���N�^
	virtual ~Object3d(); // �f�X�g���N�^
	virtual bool Init(); //������
	virtual void Update(); // �X�V
	void PlayerUpdate(Quaternion pRotMatrix);
	virtual void Draw(); // �`��
	const XMFLOAT3& GetPosition() { return position; } // ���W�̎擾
	const XMFLOAT3& GetRotation() { return rotation; } // ��]�p�̎擾
	const XMFLOAT3& GetScale() { return scale; } // ��]�p�̎擾
	const XMFLOAT4& GetColor() { return color; } // �F�̎擾
	void SetPosition(XMFLOAT3 position) { this->position = position; } // ���W�̐ݒ�
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; } // ��]�p�̐ݒ�
	void SetColor(XMFLOAT4 color) { this->color = color; } // �F�̐ݒ�
	void SetScale(XMFLOAT3 scale) { this->scale = scale; } // �X�P�[���̐ݒ�
	void SetModel(Model* model) { this->model = model; } // ���f���̃Z�b�g
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; } // �r���{�[�h�t���O�̃Z�b�g
	const XMMATRIX& GetMatWorld() { return matWorld; } // ���[���h�s��̎擾
	void SetCollider(BaseCollider* collider); // �R���C�_�[�̃Z�b�g
	virtual void OnCollision(const CollisionInfo& info) {} // �Փˎ��R�[���o�b�N�֐�

private:
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	XMFLOAT4 color = { 1,1,1,1 }; // �F
	XMFLOAT3 scale = { 1,1,1 }; // ���[�J���X�P�[��
	XMFLOAT3 rotation = { 0,0,0 }; // X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 position = { 0,0,0 }; // ���[�J�����W
	XMMATRIX matWorld;// ���[�J�����[���h�ϊ��s��
	Object3d* parent = nullptr; // �e�I�u�W�F�N�g
	Model* model = nullptr; // ���f��
	bool isBillboard = false; // �r���{�[�h

protected:
	const char* name = nullptr; //�N���X��(�f�o�b�N�p)
	BaseCollider* collider = nullptr; // �R���C�_�[
};