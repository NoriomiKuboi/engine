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
	// 頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public:
	static void StaticInit(ID3D12Device* device, int window_width, int window_height); // 初期化
	static bool LoadTexture(UINT texnumber, const wchar_t* filename); // テクスチャ読み込み
	static void BeforeDraw(ID3D12GraphicsCommandList* cmdList); // 描画前処理
	static void AfterDraw(); // 描画後処理
	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false); // スプライト生成

private:
	static const int srvCount = 512; // テクスチャの最大枚数
	static const int vertNum = 4; // 頂点数
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
	bool Init(); // 初期化
	void SetRotation(float rotation); // 角度の設定
	void SetPosition(XMFLOAT2 position); // 座標の設定
	void SetSize(XMFLOAT2 size); // サイズの設定
	void SetAnchorPoint(XMFLOAT2 anchorpoint); // アンカーポイントの設定
	void SetIsFlipX(bool isFlipX); // 左右反転の設定
	void SetIsFlipY(bool isFlipY); // 上下反転の設定
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize); // テクスチャ範囲設
	void Draw(); // 描画

private:
	ComPtr<ID3D12Resource> vertBuff; // 頂点バッファ
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{}; // 頂点バッファビュー
	UINT texNumber = 0; // テクスチャ番号
	float rotation = 0.0f; // Z軸回りの回転角
	XMFLOAT2 position{}; // 座標
	XMFLOAT2 size = { 100.0f, 100.0f }; // スプライト幅、高さ
	XMFLOAT2 anchorpoint = { 0, 0 }; // アンカーポイント
	XMMATRIX matWorld{}; // ワールド行列
	XMFLOAT4 color = { 1, 1, 1, 1 }; // 色
	bool isFlipX = false; // 左右反転
	bool isFlipY = false; // 上下反転
	XMFLOAT2 texBase = { 0, 0 }; // テクスチャ始点
	XMFLOAT2 texSize = { 100.0f, 100.0f }; // テクスチャ幅、高さ

private:
	void TransferVertices(); // 頂点データ転送
};

