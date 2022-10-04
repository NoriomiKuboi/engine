#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>

#include "ObjectFbx.h"

using namespace DirectX;

GameScene::GameScene()
{
	pos = { 0.0f,0.0f,0.0f };
	subPos = { 0.0f,0.0f,-5.0f };
	random = true;
	trigger1 = false;
	trigger2 = false;
	vec = 0.0f;
	degree = 0.0f;
}

GameScene::~GameScene()
{
	safe_delete(sprite1);
	safe_delete(particleMan);
	safe_delete(modelSample);
	safe_delete(modelFbx);
	safe_delete(light);
	safe_delete(objFbx);
	safe_delete(perlin);
	for (int j = 0;j < cubeNum;j++)
	{
		for (int i = 0;i < cubeNum;i++)
		{
			safe_delete(objSample[j][i]);
		}
	}
	safe_delete(obj);
}

void GameScene::Init(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
		return;
	}

	// デバッグテキスト初期化
	debugText.Init(debugTextTexNumber);
	
	// 乱数初期化
	//srand(time(NULL));

	// カメラ生成
	camera = new CameraSub(WindowApp::winWidth, WindowApp::winHeight, input);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/tex1.png"))
	{
		assert(0);
		return;
	}

	// 背景スプライト生成
	sprite1 = Sprite::Create(1, { 0.0f,0.0f });

	// パーティクルマネージャ生成
	particleMan = ParticleManager::Create(dxCommon->GetDev(), camera);

	// モデル読み込み
	modelSample = Model::CreateFromOBJ("cubeSample");

	// 3Dオブジェクト生成
	for (int j = 0;j < cubeNum;j++)
	{
		for (int i = 0;i < cubeNum;i++)
		{
			objSample[j][i] = Object3d::Create(modelSample);
		}
	}

	obj = Object3d::Create(modelSample);

	// カメラ注視点をセット
	//camera->SetTarget({ 25.0f, 25.0f, 0 });
	//camera->SetDistance(75.0f);

	camera->SetTarget({ 6.0f, 6.0f, 0 });
	camera->SetDistance(20.0f);

	//ライトの生成
	light = Light::Create();

	Audio::SoundData sound = audio->SoundLoadWave("Resources/bgm1.wav");
	//audio->SoundPlayWava(sound,true);

	// モデル名を指定してファイル読み込み
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");
	modelFbx = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// デバイスをセット
	ObjectFbx::SetDev(dxCommon->GetDev());
	// カメラをセット
	ObjectFbx::SetCamera(camera);
	// グラフィックスパイプライン生成
	ObjectFbx::CreateGraphicsPipeline();

	// FBX生成とモデルのセット
	objFbx = new ObjectFbx;
	objFbx->Init();
	objFbx->SetModel(modelFbx);

	perlin = new Noise;
	perlin->CreateRandom(0);
}

void GameScene::Update()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	// パーティクル生成
	CreateParticles();

	//光線方向初期化
	static XMVECTOR lightDir = { 0,1,10,0 };
	light->SetLightDir(lightDir);

	std::ostringstream debugstr;
	//debugstr.str("");
	//debugstr.clear();
	//const XMFLOAT3& cameraPos = camera->GetEye();
	//debugstr << "cameraPos("
	//	<< std::fixed << std::setprecision(2)
	//	<< cameraPos.x << ","
	//	<< cameraPos.y << ","
	//	<< cameraPos.z << ")";
	//debugText.Print(debugstr.str(), 30, 50, 1.0f);

	camera->Update();
	particleMan->Update();

	//ライトの色を設定
	light->SetLightColor({ 1,1,1 });
	//3Dオブジェクトにライトを設定
	Object3d::SetLight(light);
	for (int x = 0; x < cubeNum; x++)
	{
		for (int y = 0; y < cubeNum; y++)
		{
			pos.x = float(x) * 1.5f;
			pos.y = float(y) * 1.5f;
			pos.z = perlin->Perlin(pos.x, pos.y);
			objSample[x][y]->SetScale({ 0.5f,0.5f,0.5f });
			objSample[x][y]->SetColor({ 0,0.4f,0,1 });
			objSample[x][y]->SetPosition(pos);
		}
	}

	debugstr.str("");
	debugstr.clear();
	const float padRot = Xinput->GetPadLStickAngle();
	debugstr << "padRot("
		<< std::fixed << std::setprecision(1)
		<< padRot << ")";
	debugText.Print(debugstr.str(), 30, 50, 1.0f);

	if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false) || Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		float padRot = Xinput->GetPadLStickAngle();
		float angle = XMConvertToRadians(padRot);
		subPos.x += 0.3f * cosf(angle);
		subPos.y += 0.3f * sinf(angle);
	}

	/*if (random == true)
	{
		for (int x = 0;x < cubeNum;x++)
		{
			for (int y = 0;y < cubeNum;y++)
			{
				pos.x = x * 10.0f;
				pos.y = y * 10.0f;
				pos.z = rand() % 60 - 30;
				if (pos.z <= 0) { pos.z = 10000; }
				objSample[x][y]->SetScale({ 5.0f,5.0f,5.0f });
				objSample[x][y]->SetPosition(pos);
			}
		}

		random = false;
	}*/

	for (int x = 0;x < cubeNum;x++)
	{
		for (int y = 0;y < cubeNum;y++)
		{
			objSample[x][y]->Update();
		}
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		trigger1 = true;
	}

	if (trigger1 == true)
	{
		vec += g;
		subPos.z += vec;
	}

	if (subPos.z > 20.0f)
	{
		subPos.z = -5.0f;
		vec = 0.0f;
		trigger1 = false;
		trigger2 = false;
	}

	if (subPos.z > 0.5f)
	{
		obj->SetScale({ 0.1f,0.1f,0.1f });
	}

	else
	{
		obj->SetScale({ 0.5f,0.5f,0.5f });
	}

	obj->SetPosition(subPos);

	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();

	objFbx->Update();

	light->Update();

	Xinput = nullptr;
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	// 背景スプライト描画前処理
	Sprite::BeforeDraw(cmdList);

	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::AfterDraw();

	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();

	// 3Dオブジェクトの描画前処理
	Object3d::BeforeDraw(cmdList);

	// 3Dオブジェクト描画
	for (int j = 0;j < cubeNum;j++)
	{
		for (int i = 0;i < cubeNum;i++)
		{
			objSample[j][i]->Draw();
		}
	}

	obj->Draw();

	// 3Dオブジェクトの描画後処理
	Object3d::AfterDraw();

	//objFbx->Draw(cmdList);
	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// 前景スプライト描画前処理
	Sprite::BeforeDraw(cmdList);

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::AfterDraw();
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++)
	{
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//色をランダムに分布
		XMFLOAT4 color;
		color.w = 1;
		color.x = (float)rand() / RAND_MAX * 1;
		color.y = (float)rand() / RAND_MAX * 1;
		color.z = (float)rand() / RAND_MAX * 1;

		// 追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}
