#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>

#include "ObjectFbx.h"

using namespace DirectX;

GameScene::GameScene()
{
	blockPos = { 0.0f,0.0f,0.0f };
	pPos = { 6.0f,6.0f,-5.0f };
	pBullPos = pPos;
	random = true;
	trigger1 = false;
	trigger2 = false;
	vec = 0.0f;
	scene = SceneName::title;

	sec = 0;
	time = 0;
	count = 0;

	speed = 0.1f;
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
	qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);
}

GameScene::~GameScene()
{
	safe_delete(sprite1);
	safe_delete(titleBack);
	safe_delete(gameBack);
	safe_delete(endBack);
	safe_delete(particleMan);
	safe_delete(light);
	safe_delete(perlin);
	safe_delete(operation);
	safe_delete(timer);
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

	if (!Sprite::LoadTexture(2, L"Resources/titleSample.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(3, L"Resources/back.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(4, L"Resources/endSample.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(5, L"Resources/operation.png"))
	{
		assert(0);
		return;
	}

	// 背景スプライト生成
	sprite1 = Sprite::Create(1, { 0.0f,0.0f });
	titleBack = Sprite::Create(2, { 0.0f,0.0f });
	gameBack = Sprite::Create(3, { 0.0f,0.0f });
	endBack = Sprite::Create(4, { 0.0f,0.0f });
	operation = Sprite::Create(5, { 0.0f,0.0f });

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

	timer = new Timer;
	timer->Init();
}

void GameScene::Update()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	const XMFLOAT3& cameraPos = camera->GetEye();

	ImGui::Begin("Info");
	ImGui::SetWindowPos(ImVec2(20, 20), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(210, 110), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::Text("LSticlRot %.1f", Xinput->GetPadLStickAngle());
	ImGui::Text("RSticlRot %.1f", Xinput->GetPadRStickAngle());
	ImGui::Text("cameraPos( %.1f, %.1f, %.1f )", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::End();

	// パーティクル生成
	CreateParticles();

	//光線方向初期化
	static XMVECTOR lightDir = { 0,1,10,0 };
	light->SetLightDir(lightDir);

	particleMan->Update();

	//ライトの色を設定
	light->SetLightColor({ 1,1,1 });

	//3Dオブジェクトにライトを設定
	Object3d::SetLight(light);

	if (scene == title)
	{
		camera->SetTarget({ 6.5f, 6.5f, 0 });
		camera->SetDistance(20.0f);

		blockPos = { 0.0f,0.0f,0.0f };
		pPos = { 6.5f,6.5f,-5.0f };
		pBullPos = pPos;
		random = true;
		trigger1 = false;
		trigger2 = false;
		vec = 0.0f;

		sec = 0;
		time = 0;
		count = 0;

		timer->Reset();

		if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A) || input->TriggerKey(DIK_RETURN))
		{
			scene = game;
		}
	}

	else if (scene == game)
	{
		camera->Update();

		if (sec < 3)
		{
			time++;
		}

		if (sec == 3)
		{
			count++;
		}

		if (count == 70)
		{
			scene = SceneName::end;
		}

		sec = time / (45 * 10) % 6;

		for (int x = 0; x < cubeNum; x++)
		{
			for (int y = 0; y < cubeNum; y++)
			{
				blockPos.x = float(x) * 1.5f;
				blockPos.y = float(y) * 1.5f;
				blockPos.z = perlin->Perlin(blockPos.x, blockPos.y);
				sampleBlock[x][y]->SetScale({ 0.8f,0.8f,0.8f });
				//sampleBlock[x][y]->SetPosition(blockPos);
			}
		}

		block();

		if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false) || Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
		{
			float LSticlRot = Xinput->GetPadLStickAngle();
			float angle = XMConvertToRadians(LSticlRot);
			pPos.x += 0.3f * cosf(angle);
			pPos.y += 0.3f * sinf(angle);
		}

		//else if (input->PushKey(DIK_W))
		//{
		//	pPos.y += 0.3f;
		//}
		//
		//else if (input->PushKey(DIK_S))
		//{
		//	pPos.y -= 0.3f;
		//}
		//
		//else if (input->PushKey(DIK_A))
		//{
		//	pPos.x -= 0.3f;
		//}
		//
		//else if (input->PushKey(DIK_D))
		//{
		//	pPos.x += 0.3f;
		//}

		rotX = 0.0f;
		rotY = 0.0f;
		rotZ = 0.0f;

		if (input->PushKey(DIK_E)) { rotX -= ROT_UINT; }
		if (input->PushKey(DIK_Z)) { rotX += ROT_UINT; }
		if (input->PushKey(DIK_A)) { rotY -= ROT_UINT; }
		if (input->PushKey(DIK_D)) { rotY += ROT_UINT; }

		if (input->PushKey(DIK_W))
		{
			speed += 0.01f;
		}

		if (input->PushKey(DIK_S))
		{
			speed -= 0.01f;

			if (speed < 0.0f)
			{
				speed = 0.0f;
			}
		}

		XMFLOAT3 vSideAxis = getAxis(quaternion(UnitX, qLocal));
		XMFLOAT3 vUpAxis = getAxis(quaternion(UnitY, qLocal));
		XMFLOAT3 vForwardAxis = getAxis(quaternion(UnitZ, qLocal));

		Quaternion qRoll = quaternion(vUpAxis, rotY);
		Quaternion qPitch = quaternion(vSideAxis, rotX);
		Quaternion qYow = quaternion(vForwardAxis, rotZ);

		qLocal = qRoll * qLocal;
		qLocal = qPitch * qLocal;
		qLocal = qYow * qLocal;

		pPos.x += vForwardAxis.x * speed;
		pPos.y += vForwardAxis.y * speed;
		pPos.z += vForwardAxis.z * speed;

		samplePlayer->PlayerUpdate(qLocal);

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
		samplePlayer->SetScale({ 0.5f,0.5f,0.5f });
		samplePlayer->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//samplePlayer->Update();

		sampleBullet->SetPosition(pBullPos);
		sampleBullet->SetScale({ 0.5f,0.5f,0.5f });
		sampleBullet->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		sampleBullet->Update();

		objFbx->Update();

		light->Update();

		timer->Update();

		operation->SetPosition({ 50.0f,550.0f });
	}

	else if (scene == end)
	{
		if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A) || input->TriggerKey(DIK_RETURN))
		{
			scene = title;
		}
	}

	Xinput = nullptr;
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	if (scene == title)
	{
		// 背景スプライト描画前処理
		Sprite::BeforeDraw(cmdList);

		// 背景スプライト描画
		titleBack->Draw();

		// スプライト描画後処理
		Sprite::AfterDraw();

		// 深度バッファクリア
		dxCommon->ClearDepthBuffer();
	}

	else if (scene == game)
	{
		// 背景スプライト描画前処理
		Sprite::BeforeDraw(cmdList);

		// 背景スプライト描画
		gameBack->Draw();

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
				//sampleBlock[j][i]->Draw();
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

		timer->Draw();
		
		operation->Draw();

		// デバッグテキストの描画
		debugText.DrawAll(cmdList);

		// 前景スプライト描画後処理
		Sprite::AfterDraw();
	}

	else if (scene == end)
	{
		// 背景スプライト描画前処理
		Sprite::BeforeDraw(cmdList);

		// 背景スプライト描画
		endBack->Draw();

		// スプライト描画後処理
		Sprite::AfterDraw();

		// 深度バッファクリア
		dxCommon->ClearDepthBuffer();
	}
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

void GameScene::block()
{
	sampleBlock[0][0]->SetPosition({ 1.5f * 0,0,0 });
	sampleBlock[0][1]->SetPosition({ 1.5f * 1,0,1 });
	sampleBlock[0][2]->SetPosition({ 1.5f * 2,0,1 });
	sampleBlock[0][3]->SetPosition({ 1.5f * 3,0,2 });
	sampleBlock[0][4]->SetPosition({ 1.5f * 4,0,2 });
	sampleBlock[0][5]->SetPosition({ 1.5f * 5,0,2 });
	sampleBlock[0][6]->SetPosition({ 1.5f * 6,0,2 });
	sampleBlock[0][7]->SetPosition({ 1.5f * 7,0,1 });
	sampleBlock[0][8]->SetPosition({ 1.5f * 8,0,1 });
	sampleBlock[0][9]->SetPosition({ 1.5f * 9,0,0 });

	sampleBlock[1][0]->SetPosition({ 1.5f * 0,1.5f * 1,0 });
	sampleBlock[1][1]->SetPosition({ 1.5f * 1,1.5f * 1,1 });
	sampleBlock[1][2]->SetPosition({ 1.5f * 2,1.5f * 1,1 });
	sampleBlock[1][3]->SetPosition({ 1.5f * 3,1.5f * 1,2 });
	sampleBlock[1][4]->SetPosition({ 1.5f * 4,1.5f * 1,2 });
	sampleBlock[1][5]->SetPosition({ 1.5f * 5,1.5f * 1,2 });
	sampleBlock[1][6]->SetPosition({ 1.5f * 6,1.5f * 1,2 });
	sampleBlock[1][7]->SetPosition({ 1.5f * 7,1.5f * 1,1 });
	sampleBlock[1][8]->SetPosition({ 1.5f * 8,1.5f * 1,1 });
	sampleBlock[1][9]->SetPosition({ 1.5f * 9,1.5f * 1,0 });

	sampleBlock[2][0]->SetPosition({ 1.5f * 0,1.5f * 2,0 });
	sampleBlock[2][1]->SetPosition({ 1.5f * 1,1.5f * 2,1 });
	sampleBlock[2][2]->SetPosition({ 1.5f * 2,1.5f * 2,1 });
	sampleBlock[2][3]->SetPosition({ 1.5f * 3,1.5f * 2,1 });
	sampleBlock[2][4]->SetPosition({ 1.5f * 4,1.5f * 2,2 });
	sampleBlock[2][5]->SetPosition({ 1.5f * 5,1.5f * 2,2 });
	sampleBlock[2][6]->SetPosition({ 1.5f * 6,1.5f * 2,1 });
	sampleBlock[2][7]->SetPosition({ 1.5f * 7,1.5f * 2,1 });
	sampleBlock[2][8]->SetPosition({ 1.5f * 8,1.5f * 2,1 });
	sampleBlock[2][9]->SetPosition({ 1.5f * 9,1.5f * 2,0 });

	sampleBlock[3][0]->SetPosition({ 1.5f * 0,1.5f * 3,0 });
	sampleBlock[3][1]->SetPosition({ 1.5f * 1,1.5f * 3,0 });
	sampleBlock[3][2]->SetPosition({ 1.5f * 2,1.5f * 3,1 });
	sampleBlock[3][3]->SetPosition({ 1.5f * 3,1.5f * 3,1 });
	sampleBlock[3][4]->SetPosition({ 1.5f * 4,1.5f * 3,2 });
	sampleBlock[3][5]->SetPosition({ 1.5f * 5,1.5f * 3,1 });
	sampleBlock[3][6]->SetPosition({ 1.5f * 6,1.5f * 3,1 });
	sampleBlock[3][7]->SetPosition({ 1.5f * 7,1.5f * 3,0 });
	sampleBlock[3][8]->SetPosition({ 1.5f * 8,1.5f * 3,0 });
	sampleBlock[3][9]->SetPosition({ 1.5f * 9,1.5f * 3,0 });

	sampleBlock[4][0]->SetPosition({ 1.5f * 0,1.5f * 4,0 });
	sampleBlock[4][1]->SetPosition({ 1.5f * 1,1.5f * 4,0 });
	sampleBlock[4][2]->SetPosition({ 1.5f * 2,1.5f * 4,0 });
	sampleBlock[4][3]->SetPosition({ 1.5f * 3,1.5f * 4,1 });
	sampleBlock[4][4]->SetPosition({ 1.5f * 4,1.5f * 4,1 });
	sampleBlock[4][5]->SetPosition({ 1.5f * 5,1.5f * 4,1 });
	sampleBlock[4][6]->SetPosition({ 1.5f * 6,1.5f * 4,0 });
	sampleBlock[4][7]->SetPosition({ 1.5f * 7,1.5f * 4,0 });
	sampleBlock[4][8]->SetPosition({ 1.5f * 8,1.5f * 4,-1 });
	sampleBlock[4][9]->SetPosition({ 1.5f * 9,1.5f * 4,-1 });

	sampleBlock[5][0]->SetPosition({ 1.5f * 0,1.5f * 5,0 });
	sampleBlock[5][1]->SetPosition({ 1.5f * 1,1.5f * 5,0 });
	sampleBlock[5][2]->SetPosition({ 1.5f * 2,1.5f * 5,0 });
	sampleBlock[5][3]->SetPosition({ 1.5f * 3,1.5f * 5,0 });
	sampleBlock[5][4]->SetPosition({ 1.5f * 4,1.5f * 5,0 });
	sampleBlock[5][5]->SetPosition({ 1.5f * 5,1.5f * 5,0 });
	sampleBlock[5][6]->SetPosition({ 1.5f * 6,1.5f * 5,0 });
	sampleBlock[5][7]->SetPosition({ 1.5f * 7,1.5f * 5,-1 });
	sampleBlock[5][8]->SetPosition({ 1.5f * 8,1.5f * 5,-1 });
	sampleBlock[5][9]->SetPosition({ 1.5f * 9,1.5f * 5,-1 });

	sampleBlock[6][0]->SetPosition({ 1.5f * 0,1.5f * 6,0 });
	sampleBlock[6][1]->SetPosition({ 1.5f * 1,1.5f * 6,0 });
	sampleBlock[6][2]->SetPosition({ 1.5f * 2,1.5f * 6,0 });
	sampleBlock[6][3]->SetPosition({ 1.5f * 3,1.5f * 6,0 });
	sampleBlock[6][4]->SetPosition({ 1.5f * 4,1.5f * 6,0 });
	sampleBlock[6][5]->SetPosition({ 1.5f * 5,1.5f * 6,0 });
	sampleBlock[6][6]->SetPosition({ 1.5f * 6,1.5f * 6,0 });
	sampleBlock[6][7]->SetPosition({ 1.5f * 7,1.5f * 6,-1 });
	sampleBlock[6][8]->SetPosition({ 1.5f * 8,1.5f * 6,-1 });
	sampleBlock[6][9]->SetPosition({ 1.5f * 9,1.5f * 6,-1 });

	sampleBlock[7][0]->SetPosition({ 1.5f * 0,1.5f * 7,0 });
	sampleBlock[7][1]->SetPosition({ 1.5f * 1,1.5f * 7,0 });
	sampleBlock[7][2]->SetPosition({ 1.5f * 2,1.5f * 7,0 });
	sampleBlock[7][3]->SetPosition({ 1.5f * 3,1.5f * 7,0 });
	sampleBlock[7][4]->SetPosition({ 1.5f * 4,1.5f * 7,0 });
	sampleBlock[7][5]->SetPosition({ 1.5f * 5,1.5f * 7,0 });
	sampleBlock[7][6]->SetPosition({ 1.5f * 6,1.5f * 7,-1 });
	sampleBlock[7][7]->SetPosition({ 1.5f * 7,1.5f * 7,-1 });
	sampleBlock[7][8]->SetPosition({ 1.5f * 8,1.5f * 7,-1 });
	sampleBlock[7][9]->SetPosition({ 1.5f * 9,1.5f * 7,-1 });

	sampleBlock[8][0]->SetPosition({ 1.5f * 0,1.5f * 8,0 });
	sampleBlock[8][1]->SetPosition({ 1.5f * 1,1.5f * 8,0 });
	sampleBlock[8][2]->SetPosition({ 1.5f * 2,1.5f * 8,0 });
	sampleBlock[8][3]->SetPosition({ 1.5f * 3,1.5f * 8,0 });
	sampleBlock[8][4]->SetPosition({ 1.5f * 4,1.5f * 8,0 });
	sampleBlock[8][5]->SetPosition({ 1.5f * 5,1.5f * 8,-1 });
	sampleBlock[8][6]->SetPosition({ 1.5f * 6,1.5f * 8,-1 });
	sampleBlock[8][7]->SetPosition({ 1.5f * 7,1.5f * 8,-1 });
	sampleBlock[8][8]->SetPosition({ 1.5f * 8,1.5f * 8,-1 });
	sampleBlock[8][9]->SetPosition({ 1.5f * 9,1.5f * 8,-1 });

	sampleBlock[9][0]->SetPosition({ 1.5f * 0,1.5f * 9,0 });
	sampleBlock[9][1]->SetPosition({ 1.5f * 1,1.5f * 9,0 });
	sampleBlock[9][2]->SetPosition({ 1.5f * 2,1.5f * 9,0 });
	sampleBlock[9][3]->SetPosition({ 1.5f * 3,1.5f * 9,-1 });
	sampleBlock[9][4]->SetPosition({ 1.5f * 4,1.5f * 9,-1 });
	sampleBlock[9][5]->SetPosition({ 1.5f * 5,1.5f * 9,-1 });
	sampleBlock[9][6]->SetPosition({ 1.5f * 6,1.5f * 9,-1 });
	sampleBlock[9][7]->SetPosition({ 1.5f * 7,1.5f * 9,-1 });
	sampleBlock[9][8]->SetPosition({ 1.5f * 8,1.5f * 9,-1 });
	sampleBlock[9][9]->SetPosition({ 1.5f * 9,1.5f * 9,-1 });
}
