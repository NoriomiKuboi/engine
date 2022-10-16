#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>

#include "ObjectFbx.h"

using namespace DirectX;

GameScene::GameScene()
{
	blockPos = { 0.0f,0.0f,0.0f };
	pPos = { 0.0f,0.0f,-5.0f };
	pBullPos = pPos;
	random = true;
	trigger1 = false;
	trigger2 = false;
	vec = 0.0f;
}

GameScene::~GameScene()
{
	safe_delete(sprite1);
	safe_delete(particleMan);
	safe_delete(light);
	safe_delete(perlin);
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
	modelSampleCube = std::make_unique<Model>();
	modelSampleCube = Model::CreateFromOBJ("cubeSample");
	modelSampleBullet = std::make_unique<Model>();
	modelSampleBullet = Model::CreateFromOBJ("bulletSample");

	// 3Dオブジェクト生成
	for (int j = 0;j < cubeNum;j++)
	{
		for (int i = 0;i < cubeNum;i++)
		{
			sampleBlock[j][i] = std::make_unique<Object3d>();
			sampleBlock[j][i] = Object3d::Create(modelSampleCube.get());
		}
	}
	samplePlayer = std::make_unique<Object3d>();
	samplePlayer = Object3d::Create(modelSampleCube.get());
	sampleBullet = std::make_unique<Object3d>();
	sampleBullet = Object3d::Create(modelSampleBullet.get());

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
	modelFbx = std::make_unique<Model>();
	modelFbx = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// デバイスをセット
	ObjectFbx::SetDev(dxCommon->GetDev());
	// カメラをセット
	ObjectFbx::SetCamera(camera);
	// グラフィックスパイプライン生成
	ObjectFbx::CreateGraphicsPipeline();

	// FBX生成とモデルのセット
	objFbx = std::make_unique<ObjectFbx>();
	objFbx->Init();
	objFbx->SetModel(modelFbx.get());

	perlin = new Noise;
	perlin->CreateRandom(0);
}

void GameScene::Update()
{
	ImGui::Begin("test");
	ImGui::SetWindowSize(ImVec2(200, 300), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::End();

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
			blockPos.x = float(x) * 1.5f;
			blockPos.y = float(y) * 1.5f;
			blockPos.z = perlin->Perlin(blockPos.x, blockPos.y);
			sampleBlock[x][y]->SetScale({ 0.5f,0.5f,0.5f });
			//sampleBlock[x][y]->SetColor({ 0,0.4f,0,1 });
			sampleBlock[x][y]->SetPosition(blockPos);
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
		pPos.x += 0.3f * cosf(angle);
		pPos.y += 0.3f * sinf(angle);
	}

	else if (input->PushKey(DIK_W))
	{
		pPos.y += 0.5f;
	}

	else if (input->PushKey(DIK_S))
	{
		pPos.y -= 0.5f;
	}

	else if (input->PushKey(DIK_A))
	{
		pPos.x -= 0.5f;
	}

	else if (input->PushKey(DIK_D))
	{
		pPos.x += 0.5f;
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
			sampleBlock[x][y]->Update();
		}
	}

	if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_RT) || input->TriggerKey(DIK_SPACE))
	{
		trigger1 = true;
		pBullPos = pPos;
	}

	if (trigger1 == true)
	{
		vec += g;
		pBullPos.z += vec;
	}

	if (pBullPos.z > 20.0f)
	{
		pBullPos.z = -5.0f;
		vec = 0.0f;
		trigger1 = false;
		trigger2 = false;
	}

	for (int x = 0;x < cubeNum;x++)
	{
		for (int y = 0;y < cubeNum;y++)
		{
			Sphere pBullet;
			pBullet.center = { sampleBlock[x][y]->GetPosition().x,sampleBlock[x][y]->GetPosition().y,sampleBlock[x][y]->GetPosition().z,1 };
			pBullet.radius = 1;
			Box block;
			block.center = { sampleBullet->GetPosition().x,sampleBullet->GetPosition().y,sampleBullet->GetPosition().z,1 };

			if (Collision::CheckSphere2Box(pBullet, block))
			{
				sampleBlock[x][y]->SetColor({ 1,0,0,1 });
			}

			else
			{
				sampleBlock[x][y]->SetColor({ 0,0.4f,0,1 });
			}
		}
	}

	samplePlayer->SetPosition(pPos);
	samplePlayer->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	samplePlayer->SetScale({ 0.5f,0.5f,0.5f });
	samplePlayer->Update();

	sampleBullet->SetScale({ 0.5f,0.5f,0.5f });
	sampleBullet->SetPosition(pBullPos);
	sampleBullet->Update();

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
			sampleBlock[j][i]->Draw();
		}
	}

	samplePlayer->Draw();
	if (trigger1 == true)
	{
		sampleBullet->Draw();
	}

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
