#pragma once
#include "Mesh.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

//-----FBX-----//
struct Node
{
	// ���O
	std::string name;
	// ���[�J���X�P�[��
	DirectX::XMVECTOR scale = { 1,1,1,0 };
	// ���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	// ���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	// ���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	// �O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	// �e�m�[�h
	Node* parent = nullptr;
};
//-------------//

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	//std::�ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	//-----FBX��������-----//
public:// �萔
	// �{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

public:
	// �{�[���\����
	struct Bone
	{
		// ���O
		std::string name;
		// �����p���̋t�s��
		DirectX::XMMATRIX invInitPose;
		// �N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		// �R���X�g���N�^
		Bone(const std::string& name) {
			this->name = name;
		}
	};

	// �t�����h�N���X
	friend class FbxLoader;
private:
	// ���f����
	std::string fbxName;
	// �m�[�h�z��
	std::vector<Node> nodes;
	// �{�[���z��
	std::vector<Bone> bones;

public:// �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; // xyz
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv; // uv
		UINT boneIndex[MAX_BONE_INDICES];// �{�[�� �ԍ�
		float boneWeight[MAX_BONE_INDICES];// �{�[���@�d��
	};

	// ���b�V�������m�[�h
	Node* meshNode = nullptr;
	// FBX�V�[��
	FbxScene* fbxScene = nullptr;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	// �A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// �f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	// �e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	// �X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};

private:// �����o�ϐ�
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:// �����o�֐�
	// �o�b�t�@����
	void CreateBuffers(ID3D12Device* device);

public:
	// �`��
	void FbxDraw(ID3D12GraphicsCommandList* cmdList);
	// ���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	// �Q�b�^�[
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
	//-----FBX�����܂�-----//

private:
	static const std::string baseDirectory;

private:
	static ID3D12Device* dev;
	static UINT descriptorHandleIncrementSize;

public:
	static void StaticInit(ID3D12Device* dev); //������
	static std::unique_ptr<Model> CreateFromOBJ(const std::string& modelname, bool smoothing = false); // OBJ�t�@�C�����烁�b�V������

public:
	~Model();
	void Init(const std::string& modelname, bool smoothing); // ������
	void Draw(ID3D12GraphicsCommandList* cmdList); // �`��

private:
	std::string name; // ���O
	std::vector<Mesh*> meshes; // ���b�V���R���e�i
	std::unordered_map<std::string, Material*> materials; // �}�e���A���R���e�i
	Material* defaultMaterial = nullptr; // �f�t�H���g�}�e���A��
	ComPtr<ID3D12DescriptorHeap> descHeap; // �f�X�N���v�^�q�[�v

private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename); // �}�e���A���ǂݍ���
	void AddMaterial(Material* material); // �}�e���A���o�^
	void CreateDescriptorHeap(); // �f�X�N���v�^�q�[�v�̐���
	void LoadTextures(); // �e�N�X�`���ǂݍ���
};