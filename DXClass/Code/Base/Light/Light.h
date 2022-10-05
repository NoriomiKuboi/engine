#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstbufferData
	{
		XMVECTOR lightv;//���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor;//���C�g�̐F
	};

private:
	static ID3D12Device* dev;

public:
	static void StaticInit(ID3D12Device* dev); // ������
	void Init(); // ������
	void TransferConstBuffer(); // �萔�o�b�t�@�]��
	void SetLightDir(const XMVECTOR& lightdir); // ���C�g�̕������Z�b�g
	void SetLightColor(const XMFLOAT3& lightcolor); // ���C�g�̐F���Z�b�g
	void Update(); // �X�V
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex); // �`��
	static Light* Create(); // �C���X�^���X����

private:
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	XMVECTOR lightdir = { 1,0,0,0 }; // ���C�g��������(�P�ʃx�N�g��)
	XMFLOAT3 lightcolor = { 1,1,1 }; // ���C�g�F
	bool dirty = false; // �_�[�e�B�t���O
};