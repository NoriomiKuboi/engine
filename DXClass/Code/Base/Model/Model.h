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
	// 名前
	std::string name;
	// ローカルスケール
	DirectX::XMVECTOR scale = { 1,1,1,0 };
	// ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	// ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	// ローカル変形行列
	DirectX::XMMATRIX transform;
	// グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	// 親ノード
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

	//std::省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	//-----FBXここから-----//
public:// 定数
	// ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

public:
	// ボーン構造体
	struct Bone
	{
		// 名前
		std::string name;
		// 初期姿勢の逆行列
		DirectX::XMMATRIX invInitPose;
		// クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		// コンストラクタ
		Bone(const std::string& name) {
			this->name = name;
		}
	};

	// フレンドクラス
	friend class FbxLoader;
private:
	// モデル名
	std::string fbxName;
	// ノード配列
	std::vector<Node> nodes;
	// ボーン配列
	std::vector<Bone> bones;

public:// サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; // xyz
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv; // uv
		UINT boneIndex[MAX_BONE_INDICES];// ボーン 番号
		float boneWeight[MAX_BONE_INDICES];// ボーン　重み
	};

	// メッシュを持つノード
	Node* meshNode = nullptr;
	// FBXシーン
	FbxScene* fbxScene = nullptr;
	// 頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;

	// アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	// テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	// スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};

private:// メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:// メンバ関数
	// バッファ生成
	void CreateBuffers(ID3D12Device* device);

public:
	// 描画
	void FbxDraw(ID3D12GraphicsCommandList* cmdList);
	// モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	// ゲッター
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
	//-----FBXここまで-----//

private:
	static const std::string baseDirectory;

private:
	static ID3D12Device* dev;
	static UINT descriptorHandleIncrementSize;

public:
	static void StaticInit(ID3D12Device* dev); //初期化
	static std::unique_ptr<Model> CreateFromOBJ(const std::string& modelname, bool smoothing = false); // OBJファイルからメッシュ生成

public:
	~Model();
	void Init(const std::string& modelname, bool smoothing); // 初期化
	void Draw(ID3D12GraphicsCommandList* cmdList); // 描画

private:
	std::string name; // 名前
	std::vector<Mesh*> meshes; // メッシュコンテナ
	std::unordered_map<std::string, Material*> materials; // マテリアルコンテナ
	Material* defaultMaterial = nullptr; // デフォルトマテリアル
	ComPtr<ID3D12DescriptorHeap> descHeap; // デスクリプタヒープ

private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename); // マテリアル読み込み
	void AddMaterial(Material* material); // マテリアル登録
	void CreateDescriptorHeap(); // デスクリプタヒープの生成
	void LoadTextures(); // テクスチャ読み込み
};