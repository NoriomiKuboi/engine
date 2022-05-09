#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// ���_�f�[�^�\����
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

public:
	static void StaticInit(ID3D12Device* device, int window_width, int window_height); // ������
	static bool LoadTexture(UINT texnumber, const wchar_t* filename); // �e�N�X�`���ǂݍ���
	static void BeforeDraw(ID3D12GraphicsCommandList* cmdList); // �`��O����
	static void AfterDraw(); // �`��㏈��
	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false); // �X�v���C�g����

private:
	static const int srvCount = 512; // �e�N�X�`���̍ő喇��
	static const int vertNum = 4; // ���_��
	static ID3D12Device* dev;
	static UINT descriptorHandleIncrementSize;
	static ID3D12GraphicsCommandList* cmdList;
	static ComPtr<ID3D12RootSignature> rootSignature;
	static ComPtr<ID3D12PipelineState> pipelineState;
	static XMMATRIX matProjection;
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	static ComPtr<ID3D12Resource> texBuff[srvCount];

public:
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	bool Init(); // ������
	void SetRotation(float rotation); // �p�x�̐ݒ�
	void SetPosition(XMFLOAT2 position); // ���W�̐ݒ�
	void SetSize(XMFLOAT2 size); // �T�C�Y�̐ݒ�
	void SetAnchorPoint(XMFLOAT2 anchorpoint); // �A���J�[�|�C���g�̐ݒ�
	void SetIsFlipX(bool isFlipX); // ���E���]�̐ݒ�
	void SetIsFlipY(bool isFlipY); // �㉺���]�̐ݒ�
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize); // �e�N�X�`���͈͐�
	void Draw(); // �`��

private:
	ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[
	UINT texNumber = 0; // �e�N�X�`���ԍ�
	float rotation = 0.0f; // Z�����̉�]�p
	XMFLOAT2 position{}; // ���W
	XMFLOAT2 size = { 100.0f, 100.0f }; // �X�v���C�g���A����
	XMFLOAT2 anchorpoint = { 0, 0 }; // �A���J�[�|�C���g
	XMMATRIX matWorld{}; // ���[���h�s��
	XMFLOAT4 color = { 1, 1, 1, 1 }; // �F
	bool isFlipX = false; // ���E���]
	bool isFlipY = false; // �㉺���]
	XMFLOAT2 texBase = { 0, 0 }; // �e�N�X�`���n�_
	XMFLOAT2 texSize = { 100.0f, 100.0f }; // �e�N�X�`�����A����

private:
	void TransferVertices(); // ���_�f�[�^�]��
};

