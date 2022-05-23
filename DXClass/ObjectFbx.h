#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"
#include "FbxLoader.h"

class ObjectFbx
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// 静的メンバ関数
	static void SetDev(ID3D12Device* dev) { ObjectFbx::dev = dev; }
	static void SetCamera(Camera* camera) { ObjectFbx::camera = camera; }

private:
	static ID3D12Device* dev;
	static Camera* camera;

public:// サブクラス
	// 定数バッファ用データ構造体(座標変換行列)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;// ビュープロジェクション行列
		XMMATRIX world;// ワールド行列
		XMFLOAT3 cameraPos;// カメラ座標(ワールド座標)
	};

public:// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

public:// 静的メンバ関数
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	static void CreateGraphicsPipeline();

private:// メンバ関数
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public:// 定数
	// ボーンの最大数
	static const int MAX_BONES = 32;

	// 1フレームの時間
	FbxTime frameTime;

	// アニメーション開始時間
	FbxTime startTime;
	
	// アニメーション終了時間
	FbxTime endTime;
	
	// 現在時間（アニメーション）
	FbxTime currentTime;

	// アニメーション再生中
	bool isPlay = false;

	// 定数バッファ用データ構造体
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転軸
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換座標
	XMMATRIX matWorld;
	// モデル
	Model* model = nullptr;
};