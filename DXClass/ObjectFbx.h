#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"
#include "FbxLoader.h"

class ObjectFbx
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �ÓI�����o�֐�
	static void SetDev(ID3D12Device* dev) { ObjectFbx::dev = dev; }
	static void SetCamera(Camera* camera) { ObjectFbx::camera = camera; }

private:
	static ID3D12Device* dev;
	static Camera* camera;

public:// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;// �r���[�v���W�F�N�V�����s��
		XMMATRIX world;// ���[���h�s��
		XMFLOAT3 cameraPos;// �J�������W(���[���h���W)
	};

public:// �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();

public:// �ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipeline();

private:// �����o�֐�
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public:// �萔
	// �{�[���̍ő吔
	static const int MAX_BONES = 32;

	// 1�t���[���̎���
	FbxTime frameTime;

	// �A�j���[�V�����J�n����
	FbxTime startTime;
	
	// �A�j���[�V�����I������
	FbxTime endTime;
	
	// ���ݎ��ԁi�A�j���[�V�����j
	FbxTime currentTime;

	// �A�j���[�V�����Đ���
	bool isPlay = false;

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]��
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ����W
	XMMATRIX matWorld;
	// ���f��
	Model* model = nullptr;
};