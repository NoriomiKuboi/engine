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
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
		XMFLOAT4 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ビュープロジェクション行列
		XMMATRIX matBillboard;	// ビルボード行列
	};

	// パーティクル1粒
	class Particle
	{
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
	public:
		XMFLOAT3 position = {}; // 座標
		XMFLOAT3 velocity = {}; // 速度
		XMFLOAT3 accel = {}; // 加速度
		XMFLOAT3 color = {}; // 色
		float scale = 1.0f; // スケール
		float rotation = 0.0f; // 回転
		XMFLOAT3 s_color = {}; // 初期値
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		XMFLOAT3 e_color = {}; // 最終値
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		int frame = 0; // 現在フレーム
		int num_frame = 0; // 終了フレーム
	};

private:
	static const int vertexCount = 1000; // 頂点数

public:
	static ParticleManager* Create(ID3D12Device* dev, Camera* camera); //インスタンス生成

public:
	void Init(); // 初期化
	void Update(); // 更新
	void Draw(ID3D12GraphicsCommandList* cmdList); // 描画
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale); // パーティクルの追加
	void InitDescriptorHeap(); // デスクリプタヒープの初期化
	void InitGraphicsPipeline(); // グラフィックパイプライン生成
	void LoadTexture(); // テクスチャ読み込み
	void CreateModel(); // モデル作成

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

