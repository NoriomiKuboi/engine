#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha;	// アルファ
	};

public:

	// 静的初期化

	static void StaticInit(ID3D12Device* device);

	// マテリアル生成
	static Material* Create();

private:
	static ID3D12Device* dev;

public:
	std::string name;	// マテリアル名
	XMFLOAT3 ambient;	// アンビエント影響度
	XMFLOAT3 diffuse;	// ディフューズ影響度
	XMFLOAT3 specular;	// スペキュラー影響度
	float alpha;		// アルファ
	std::string textureFilename;	// テクスチャファイル名

public:
	ID3D12Resource* GetConstantBuffer() { return constBuff.Get(); } // 定数バッファの取得
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle); // テクスチャ読み込み
	void Update(); // 更新

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV; }

private:
	ComPtr<ID3D12Resource> texbuff; // テクスチャバッファ
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV; // シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV; // シェーダリソースビューのハンドル(CPU)

private:
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	void Init(); // 初期化
	void CreateConstantBuffer(); // 定数バッファの生成
};

