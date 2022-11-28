#pragma once
#include "SafeDelete.h"
#include "WindowApp.h"
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

	void Init(WindowApp* win, DirectXCommon* dxCommon, Input* input, Audio* audio); // 初期化
	void Update(); // 更新
	void Draw(); // 描画
	void CreateParticles(); // パーティクル生成
	void Reticle();

private:
	WindowApp* win = nullptr;
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
	Sprite* reticle = nullptr;

	// パーティクル
	ParticleManager* particleMan = nullptr;

	// 3dObj : FBX
	static const int cubeNum = 10; // 縦横cubeの数
	static const int enemyNum = 10; // mobenemyの数
	static const int pBulletNum = 10; // playerの弾の数
	std::unique_ptr<Model> modelSampleCube;
	std::unique_ptr<Model> modelSampleBullet;
	std::unique_ptr<Model> modelFbx;
	std::unique_ptr<Object3d> samplePlayer;
	std::unique_ptr<Object3d> sampleBlock[cubeNum][cubeNum][cubeNum];
	std::unique_ptr<Object3d> sampleEnemy[enemyNum];
	std::unique_ptr<Object3d> sampleBulletEnemy[enemyNum];
	std::unique_ptr<Object3d> sampleBoss;
	std::unique_ptr<Object3d> sampleBullet[pBulletNum];
	std::unique_ptr<ObjectFbx> objFbx;

	// ライト
	Light* light = nullptr;

	// player関係変数
	XMFLOAT3 pPos; // playerの座標
	XMFLOAT3 pBullPos[pBulletNum]; // playerの弾の座標
	XMFLOAT3 rot; // playerの回転
	Quaternion qLocal; // 姿勢用クォータニオン
	int pBulletTrigger[pBulletNum]; // 弾の発射トリガー
	float speed; // playerの移動速度
	int playerCount; // 弾の発射間隔
	int pBulletAlive[pBulletNum]; // 弾の生存時間
	const float ROT_UINT = 0.05f; // 回転量
	const float vec = 2.0f; // 弾の速度
	const XMFLOAT3 UnitX = { 1.0f, 0.0f, 0.0f }; // 軸関係の定数
	const XMFLOAT3 UnitY = { 0.0f, 1.0f, 0.0f }; // 軸関係の定数
	const XMFLOAT3 UnitZ = { 0.0f, 0.0f, 1.0f }; // 軸関係の定数

	// enemy関係変数
	XMFLOAT3 ePos[enemyNum]; // mobenemyの座標
	XMFLOAT3 bPos; // bossの座標
	bool randNum; // mobenemyの座標を乱数生成
	int hit; // mobenemyに当たった数
	int hitBoss; // bossに当たった数

	// ノイズ
	Noise* perlin = nullptr;
	XMFLOAT3 blockPos;
	bool random;

	int scene;

	Timer* timer = nullptr;

	XMFLOAT2 reticlePos;

	XMFLOAT3 move;

	int count;

	XMFLOAT3 pos[enemyNum];
};