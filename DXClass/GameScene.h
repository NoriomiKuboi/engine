#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "CameraSub.h"
#include "Light.h"
#include "FbxLoader.h"
#include "ObjectFbx.h"
#include "Noise.h"

class GameScene
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const int debugTextTexNumber = 0;

public:
	GameScene();
	~GameScene();

	void Init(DirectXCommon* dxCommon, Input* input, Audio* audio); // 初期化
	void Update(); // 更新
	void Draw(); // 描画
	void CreateParticles(); // パーティクル生成

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// ゲームシーン用
	// カメラ
	CameraSub* camera = nullptr;

	// スプライト
	Sprite* sprite1 = nullptr;

	// パーティクル
	ParticleManager* particleMan = nullptr;

	// 3dObj : FBX
	static const int cubeNum = 50; // 縦横cubeの数
	Model* modelSample = nullptr;
	Model* modelFbx = nullptr;
	Object3d* objSample[cubeNum][cubeNum] = { nullptr };
	ObjectFbx* objFbx = nullptr;

	// ライト
	Light* light = nullptr;

	// ノイズ
	Noise* perlin = nullptr;
	XMFLOAT3 pos;
	bool random;

	const float RECT_SIZE = 32.0f;
	const int COLOR_MAX = 255;
	const int IMAGE_SIZE = 10;
};