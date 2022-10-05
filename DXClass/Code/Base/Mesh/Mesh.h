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
	// 頂点データ構造体（テクスチャあり）
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

public:
	static void StaticInit(ID3D12Device* dev); // 初期化

private:
	static ID3D12Device* dev;

public:
	const std::string& GetName() { return name; } // 名前を取得
	void SetName(const std::string& name); // 名前をセット
	void AddVertex(const VertexPosNormalUv& vertex); // 頂点データの追加
	void AddIndex(unsigned short index); // 頂点インデックスの追加
	inline size_t GetVertexCount() { return vertices.size(); } // 頂点データの数を取得
	Material* GetMaterial() { return material; } // マテリアルの取得
	void SetMaterial(Material* material); // マテリアルの割り当て
	void CreateBuffers();// バッファの生成
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; } // 頂点バッファ取得
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; } // インデックスバッファ取得
	void Draw(ID3D12GraphicsCommandList* cmdList); // 描画
	void AddSmoothingData(unsigned short indexPsition, unsigned short indexVertex); //エッジ平滑化データの追加
	void CalculateSmoothedVertexNormals(); //平滑化
	std::unordered_map<unsigned short, std::vector<unsigned short>>smoothData; //頂点法線スムージングデータ

private:
	std::string name;	// 名前
	ComPtr<ID3D12Resource> vertBuff; // 頂点バッファ
	ComPtr<ID3D12Resource> indexBuff; // インデックスバッファ
	D3D12_VERTEX_BUFFER_VIEW vbView = {}; // 頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {}; // インデックスバッファビュー
	std::vector<VertexPosNormalUv> vertices; // 頂点データ配列
	std::vector<unsigned short> indices; // 頂点インデックス配列
	Material* material = nullptr; // マテリアル
};