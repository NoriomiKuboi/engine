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
	// std::を省略
	using string = std::string;

public:	// 定数
	// モデル格納ルートパス
	static const string baseDirectory;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">D3D12デバイス
	void Init(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Final();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName"></param>
	//void LoadModelFromFile(const string& modelName);
	std::unique_ptr<Model> LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード攻勢を解析
	/// </summary>
	/// <param name="model"読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(Model* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxNode"></param>
	void ParseMesh(Model* model, FbxNode* fbxNode);

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	//---サブ関数---//
	// 頂点座標読み取り
	void ParseMeshVertices(Model* model, FbxMesh* fbxMesh);
	// 面情報読み取り
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	// マテリアル読み取り
	void ParseMaterial(Model* model, FbxNode* fbxNode);
	// テクスチャ読み取り
	void LoadTexture(Model* model, const std::string& fullpath);
	// スキニング情報読み取り
	void ParseSkin(Model* model, FbxMesh* fbxMesh);
	//-------------//

	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
	// D3D12デバイス
	ID3D12Device* device = nullptr;
	// FBXマネージャ
	FbxManager* fbxManager = nullptr;
	// FBXインポータ
	FbxImporter* fbxImporter = nullptr;
};