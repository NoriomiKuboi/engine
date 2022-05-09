#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 specular; // �X�y�L�����[�W��
		float alpha;	// �A���t�@
	};

public:

	// �ÓI������

	static void StaticInit(ID3D12Device* device);

	// �}�e���A������
	static Material* Create();

private:
	static ID3D12Device* dev;

public:
	std::string name;	// �}�e���A����
	XMFLOAT3 ambient;	// �A���r�G���g�e���x
	XMFLOAT3 diffuse;	// �f�B�t���[�Y�e���x
	XMFLOAT3 specular;	// �X�y�L�����[�e���x
	float alpha;		// �A���t�@
	std::string textureFilename;	// �e�N�X�`���t�@�C����

public:
	ID3D12Resource* GetConstantBuffer() { return constBuff.Get(); } // �萔�o�b�t�@�̎擾
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle); // �e�N�X�`���ǂݍ���
	void Update(); // �X�V

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV; }

private:
	ComPtr<ID3D12Resource> texbuff; // �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV; // �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV; // �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)

private:
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	void Init(); // ������
	void CreateConstantBuffer(); // �萔�o�b�t�@�̐���
};

