#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <vector>
#include <unordered_map>

using namespace DirectX;

class Mesh
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// ���_�f�[�^�\���́i�e�N�X�`������j
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

public:
	static void StaticInit(ID3D12Device* dev); // ������

private:
	static ID3D12Device* dev;

public:
	const std::string& GetName() { return name; } // ���O���擾
	void SetName(const std::string& name); // ���O���Z�b�g
	void AddVertex(const VertexPosNormalUv& vertex); // ���_�f�[�^�̒ǉ�
	void AddIndex(unsigned short index); // ���_�C���f�b�N�X�̒ǉ�
	inline size_t GetVertexCount() { return vertices.size(); } // ���_�f�[�^�̐����擾
	Material* GetMaterial() { return material; } // �}�e���A���̎擾
	void SetMaterial(Material* material); // �}�e���A���̊��蓖��
	void CreateBuffers();// �o�b�t�@�̐���
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; } // ���_�o�b�t�@�擾
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; } // �C���f�b�N�X�o�b�t�@�擾
	void Draw(ID3D12GraphicsCommandList* cmdList); // �`��
	void AddSmoothingData(unsigned short indexPsition, unsigned short indexVertex); //�G�b�W�������f�[�^�̒ǉ�
	void CalculateSmoothedVertexNormals(); //������
	std::unordered_map<unsigned short, std::vector<unsigned short>>smoothData; //���_�@���X���[�W���O�f�[�^

private:
	std::string name;	// ���O
	ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff; // �C���f�b�N�X�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView = {}; // ���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {}; // �C���f�b�N�X�o�b�t�@�r���[
	std::vector<VertexPosNormalUv> vertices; // ���_�f�[�^�z��
	std::vector<unsigned short> indices; // ���_�C���f�b�N�X�z��
	Material* material = nullptr; // �}�e���A��
};