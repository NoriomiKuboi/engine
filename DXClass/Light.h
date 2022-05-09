#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// 定数バッファ用データ構造体
	struct ConstbufferData
	{
		XMVECTOR lightv;//ライトへの方向を表すベクトル
		XMFLOAT3 lightcolor;//ライトの色
	};

private:
	static ID3D12Device* dev;

public:
	static void StaticInit(ID3D12Device* dev); // 初期化
	void Init(); // 初期化
	void TransferConstBuffer(); // 定数バッファ転送
	void SetLightDir(const XMVECTOR& lightdir); // ライトの方向をセット
	void SetLightColor(const XMFLOAT3& lightcolor); // ライトの色をセット
	void Update(); // 更新
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex); // 描画
	static Light* Create(); // インスタンス生成

private:
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	XMVECTOR lightdir = { 1,0,0,0 }; // ライト光線方向(単位ベクトル)
	XMFLOAT3 lightcolor = { 1,1,1 }; // ライト色
	bool dirty = false; // ダーティフラグ
};