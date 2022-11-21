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
#include "PostEffect.h"
#include "XInputManager.h"
#include "ImguiManager.h"
#include "CollisionPrimitive.h"
#include "CollisionManager.h"
#include "Collision.h"
#include "Quaternion.h"

#include "Timer.h"

enum SceneName
{
	title,
	game,
	end
};

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
	Sprite* titleBack = nullptr;
	Sprite* gameBack = nullptr;
	Sprite* endBack = nullptr;
	Sprite* operation = nullptr;

	// パーティクル
	ParticleManager* particleMan = nullptr;

	// 3dObj : FBX
	static const int cubeNum = 10; // 縦横cubeの数
	std::unique_ptr<Model> modelSampleCube;
	std::unique_ptr<Model> modelSampleBullet;
	std::unique_ptr<Model> modelFbx;
	std::unique_ptr<Object3d> samplePlayer;
	std::unique_ptr<Object3d> sampleBlock[cubeNum][cubeNum][cubeNum];
	std::unique_ptr<Object3d> sampleBullet;
	std::unique_ptr<ObjectFbx> objFbx;

	XMFLOAT3 pPos;
	XMFLOAT3 pBullPos;
	const float g = 9.8f / 60.0f;
	const float vec = 2.0f;
	bool trigger1;
	bool trigger2;

	// ライト
	Light* light = nullptr;

	// ノイズ
	Noise* perlin = nullptr;
	XMFLOAT3 blockPos;
	bool random;

	int scene;

	Timer* timer = nullptr;

	int sec;
	int time;
	int count;

	float speed;
	XMFLOAT3 rot;
	Quaternion qLocal;
	const float ROT_UINT = 0.03f;
	const XMFLOAT3 UnitX = { 1.0f, 0.0f, 0.0f };
	const XMFLOAT3 UnitY = { 0.0f, 1.0f, 0.0f };
	const XMFLOAT3 UnitZ = { 0.0f, 0.0f, 1.0f };
};