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
#include "XInput.h"

enum Scene
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

	void Init(DirectXCommon* dxCommon, Input* input, Audio* audio); // ������
	void Update(); // �X�V
	void Draw(); // �`��
	void CreateParticles(); // �p�[�e�B�N������

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// �Q�[���V�[���p
	// �J����
	CameraSub* camera = nullptr;

	// �X�v���C�g
	Sprite* sprite1 = nullptr;
	Sprite* titleSprite = nullptr;
	Sprite* endSprite = nullptr;
	Sprite* num0a = nullptr;
	Sprite* num1a = nullptr;
	Sprite* num2a = nullptr;
	Sprite* num3a = nullptr;
	Sprite* num0b = nullptr;
	Sprite* num1b = nullptr;
	Sprite* num2b = nullptr;
	Sprite* num3b = nullptr;
	Sprite* num4 = nullptr;
	Sprite* num5 = nullptr;
	Sprite* num6 = nullptr;
	Sprite* num7 = nullptr;
	Sprite* num8 = nullptr;
	Sprite* num9 = nullptr;
	Sprite* back = nullptr;

	// �p�[�e�B�N��
	ParticleManager* particleMan = nullptr;

	// 3dObj : FBX
	static const int cubeNum = 9; // �c��cube�̐�
	Model* modelSample = nullptr;
	Model* modelFbx = nullptr;
	Object3d* objSample[cubeNum][cubeNum] = { nullptr };
	Object3d* obj = nullptr;
	ObjectFbx* objFbx = nullptr;

	// �V�[��
	int scene = Scene::title;

	XMFLOAT3 subPos;
	const float g = 9.8f / 60.0f;
	float vec;
	bool trigger1;
	bool trigger2;

	int time;
	int secTen;
	int secOne;
	int count;

	// ���C�g
	Light* light = nullptr;

	// �m�C�Y
	Noise* perlin = nullptr;
	XMFLOAT3 pos;
	bool random;
};