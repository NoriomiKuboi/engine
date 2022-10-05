#pragma once

#include "fbxsdk.h"
#include "Model.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <assert.h>

class FbxLoader
{
private:
	// std::���ȗ�
	using string = std::string;

public:	// �萔
	// ���f���i�[���[�g�p�X
	static const string baseDirectory;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">D3D12�f�o�C�X
	void Init(ID3D12Device* device);

	/// <summary>
	/// ��n��
	/// </summary>
	void Final();

	/// <summary>
	/// �t�@�C������FBX���f���ǂݍ���
	/// </summary>
	/// <param name="modelName"></param>
	//void LoadModelFromFile(const string& modelName);
	std::unique_ptr<Model> LoadModelFromFile(const string& modelName);

	/// <summary>
	/// �ċA�I�Ƀm�[�h�U�������
	/// </summary>
	/// <param name="model"�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseNodeRecursive(Model* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxNode"></param>
	void ParseMesh(Model* model, FbxNode* fbxNode);

	/// <summary>
	/// FBX�̍s���XMMatrix�ɕϊ�
	/// </summary>
	/// <param name="dst">�������ݐ�</param>
	/// <param name="src">���ƂȂ�FBX�s��</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	//---�T�u�֐�---//
	// ���_���W�ǂݎ��
	void ParseMeshVertices(Model* model, FbxMesh* fbxMesh);
	// �ʏ��ǂݎ��
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	// �}�e���A���ǂݎ��
	void ParseMaterial(Model* model, FbxNode* fbxNode);
	// �e�N�X�`���ǂݎ��
	void LoadTexture(Model* model, const std::string& fullpath);
	// �X�L�j���O���ǂݎ��
	void ParseSkin(Model* model, FbxMesh* fbxMesh);
	//-------------//

	// �f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
	std::string ExtractFileName(const std::string& path);

	// �e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const string defaultTextureFileName;

private:
	// private�ȃR���X�g���N�^�i�V���O���g���p�^�[���j
	FbxLoader() = default;
	// private�ȃf�X�g���N�^�i�V���O���g���p�^�[���j
	~FbxLoader() = default;
	// �R�s�[�R���X�g���N�^���֎~�i�V���O���g���p�^�[���j
	FbxLoader(const FbxLoader& obj) = delete;
	// �R�s�[������Z�q���֎~�i�V���O���g���p�^�[���j
	void operator=(const FbxLoader& obj) = delete;
	// D3D12�f�o�C�X
	ID3D12Device* device = nullptr;
	// FBX�}�l�[�W��
	FbxManager* fbxManager = nullptr;
	// FBX�C���|�[�^
	FbxImporter* fbxImporter = nullptr;
};