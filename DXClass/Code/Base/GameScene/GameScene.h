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

	void Init(WindowApp* win, DirectXCommon* dxCommon, Input* input, Audio* audio); // ������
	void Update(); // �X�V
	void Draw(); // �`��
	void CreateParticles(); // �p�[�e�B�N������
	void Reticle();

private:
	WindowApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// �Q�[���V�[���p
	// �J����
	CameraSub* camera = nullptr;

	// �X�v���C�g
	Sprite* sprite1 = nullptr;
	Sprite* titleBack = nullptr;
	Sprite* gameBack = nullptr;
	Sprite* endBack = nullptr;
	Sprite* operation = nullptr;
	Sprite* reticle = nullptr;

	// �p�[�e�B�N��
	ParticleManager* particleMan = nullptr;

	// 3dObj : FBX
	static const int cubeNum = 10; // �c��cube�̐�
	static const int enemyNum = 10; // mobenemy�̐�
	static const int pBulletNum = 10; // player�̒e�̐�
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

	// ���C�g
	Light* light = nullptr;

	// player�֌W�ϐ�
	XMFLOAT3 pPos; // player�̍��W
	XMFLOAT3 pBullPos[pBulletNum]; // player�̒e�̍��W
	XMFLOAT3 rot; // player�̉�]
	Quaternion qLocal; // �p���p�N�H�[�^�j�I��
	int pBulletTrigger[pBulletNum]; // �e�̔��˃g���K�[
	float speed; // player�̈ړ����x
	int playerCount; // �e�̔��ˊԊu
	int pBulletAlive[pBulletNum]; // �e�̐�������
	const float ROT_UINT = 0.05f; // ��]��
	const float vec = 2.0f; // �e�̑��x
	const XMFLOAT3 UnitX = { 1.0f, 0.0f, 0.0f }; // ���֌W�̒萔
	const XMFLOAT3 UnitY = { 0.0f, 1.0f, 0.0f }; // ���֌W�̒萔
	const XMFLOAT3 UnitZ = { 0.0f, 0.0f, 1.0f }; // ���֌W�̒萔

	// enemy�֌W�ϐ�
	XMFLOAT3 ePos[enemyNum]; // mobenemy�̍��W
	XMFLOAT3 bPos; // boss�̍��W
	bool randNum; // mobenemy�̍��W�𗐐�����
	int hit; // mobenemy�ɓ���������
	int hitBoss; // boss�ɓ���������

	// �m�C�Y
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