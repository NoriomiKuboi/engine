#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Object3d
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// パイプラインセット
	struct PipelineSet
	{
		ComPtr<ID3D12RootSignature> rootsignature; // ルートシグネチャ
		ComPtr<ID3D12PipelineState> pipelinestate; // パイプラインステートオブジェクト
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX viewproj;// ビュープロジェクション行列
		XMMATRIX world;// ワールド行列
		XMFLOAT3 cameraPos;// カメラ座標(ワールド座標)
	};

public:
	static void StaticInit(ID3D12Device* device, Camera* camera = nullptr); // 初期化
	static void CreateGraphicsPipeline(); // グラフィックパイプラインの生成

	// カメラのセット
	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	static void BeforeDraw(ID3D12GraphicsCommandList* cmdList); // 描画前処理
	static void AfterDraw(); // 描画後処理
	static Object3d* Create(Model* model = nullptr); // 3Dオブジェクト生成
	static void SetLight(Light* light) { Object3d::light = light; } //ライトのセット

private:
	static ID3D12Device* dev;
	static ID3D12GraphicsCommandList* cmdList;
	static PipelineSet pipelineSet;
	static Camera* camera;
	static Light* light;

public:
	bool Init(); //初期化
	void Update(); // 更新
	void Draw(); // 描画
	const XMFLOAT3& GetPosition() { return position; } // 座標の取得
	const XMFLOAT3& GetRotation() { return rotation; } //回転角の取得
	void SetPosition(XMFLOAT3 position) { this->position = position; } // 座標の設定
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; } // 回転角の設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; } // スケールの設定
	void SetModel(Model* model) { this->model = model; } // モデルのセット
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; } // ビルボードフラグのセット

private:
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	XMFLOAT4 color = { 1,1,1,1 }; // 色
	XMFLOAT3 scale = { 1,1,1 }; // ローカルスケール
	XMFLOAT3 rotation = { 0,0,0 }; // X,Y,Z軸回りのローカル回転角
	XMFLOAT3 position = { 0,0,0 }; // ローカル座標
	XMMATRIX matWorld;// ローカルワールド変換行列
	Object3d* parent = nullptr; // 親オブジェクト
	Model* model = nullptr; // モデル
	bool isBillboard = false; // ビルボード
};

