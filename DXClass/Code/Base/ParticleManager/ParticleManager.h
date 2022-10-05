#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

class ParticleManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale; // �X�P�[��
		XMFLOAT4 color;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	// �p�[�e�B�N��1��
	class Particle
	{
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
	public:
		XMFLOAT3 position = {}; // ���W
		XMFLOAT3 velocity = {}; // ���x
		XMFLOAT3 accel = {}; // �����x
		XMFLOAT3 color = {}; // �F
		float scale = 1.0f; // �X�P�[��
		float rotation = 0.0f; // ��]
		XMFLOAT3 s_color = {}; // �����l
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		XMFLOAT3 e_color = {}; // �ŏI�l
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		int frame = 0; // ���݃t���[��
		int num_frame = 0; // �I���t���[��
	};

private:
	static const int vertexCount = 1000; // ���_��

public:
	static ParticleManager* Create(ID3D12Device* dev, Camera* camera); //�C���X�^���X����

public:
	void Init(); // ������
	void Update(); // �X�V
	void Draw(ID3D12GraphicsCommandList* cmdList); // �`��
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale); // �p�[�e�B�N���̒ǉ�
	void InitDescriptorHeap(); // �f�X�N���v�^�q�[�v�̏�����
	void InitGraphicsPipeline(); // �O���t�B�b�N�p�C�v���C������
	void LoadTexture(); // �e�N�X�`���ǂݍ���
	void CreateModel(); // ���f���쐬

private:
	ID3D12Device* dev = nullptr;
	UINT descriptorHandleIncrementSize = 0u;
	ComPtr<ID3D12RootSignature> rootsignature;
	ComPtr<ID3D12PipelineState> pipelinestate;
	ComPtr<ID3D12DescriptorHeap> descHeap;
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> texbuff;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	ComPtr<ID3D12Resource> constBuff;
	std::forward_list<Particle> particles;
	Camera* camera = nullptr;
private:
	ParticleManager(ID3D12Device* dev, Camera* camera);
};

