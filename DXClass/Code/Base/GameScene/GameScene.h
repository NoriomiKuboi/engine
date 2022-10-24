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
	Sprite* num0One = nullptr;
	Sprite* num1One = nullptr;
	Sprite* num2One = nullptr;
	Sprite* num3One = nullptr;
	Sprite* num0Ten = nullptr;
	Sprite* num1Ten = nullptr;
	Sprite* num2Ten = nullptr;
	Sprite* num3Ten = nullptr;
	Sprite* num4 = nullptr;
	Sprite* num5 = nullptr;
	Sprite* num6 = nullptr;
	Sprite* num7 = nullptr;
	Sprite* num8 = nullptr;
	Sprite* num9 = nullptr;
	Sprite* operation = nullptr;

	// パーティクル
	ParticleManager* particleMan = nullptr;

	// 3dObj : FBX
	static const int cubeNum = 9; // 縦横cubeの数
	std::unique_ptr<Model> modelSampleCube;
	std::unique_ptr<Model> modelSampleBullet;
	std::unique_ptr<Model> modelFbx;
	std::unique_ptr<Object3d> samplePlayer;
	std::unique_ptr<Object3d> sampleBlock[cubeNum][cubeNum];
	std::unique_ptr<Object3d> sampleBullet;
	std::unique_ptr<ObjectFbx> objFbx;

	XMFLOAT3 pPos;
	XMFLOAT3 pBullPos;
	const float g = 9.8f / 60.0f;
	float vec;
	bool trigger1;
	bool trigger2;

	int time;
	int secOne;
	int secTen;
	int count;

	// ライト
	Light* light = nullptr;

	// ノイズ
	Noise* perlin = nullptr;
	XMFLOAT3 blockPos;
	bool random;

	int scene;
};