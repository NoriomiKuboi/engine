#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include "ObjectFbx.h"

using namespace DirectX;

GameScene::GameScene()
{
	// player関係変数
	pPos = { 13.5f,13.5f,-5.0f }; // playerの座標
	rot = { 0.0f,0.0f,0.0f }; // playerの回転
	qLocal = quaternion(XMFLOAT3(0, 0, 1), 0); // 姿勢用クォータニオン
	for (int i = 0;i < pBulletNum;i++)
	{
		pBullPos[i] = pPos; // playerの弾の座標
		pBulletTrigger[i] = { 0 }; // 弾の発射トリガー
		pBulletAlive[i] = 0;  // 弾の生存時間
	}
	speed = 0.1f; // playerの移動速度
	playerCount = 0; // 弾の発射間隔

	// enemy関係変数
	for (int i = 0;i < enemyNum;i++)
	{
		ePos[i] = { 0.0f,0.0f,0.0f }; // mobenemyの座標
	}
	bPos = { 30.0f,30.0f,30.0f }; // bossの座標
	randNum = true; // mobenemyの座標を乱数生成
	hit = 0; // mobenemyに当たった数
	hitBoss = 0; // bossに当たった数

	// ノイズ
	blockPos = { 0.0f,0.0f,0.0f };
	random = true;

	// シーン関係
	scene = SceneName::title;
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
	for (int i = 0;i < enemyNum;i++)
	{
		sampleEnemy[i] = std::make_unique<Object3d>();
		sampleEnemy[i] = Object3d::Create(modelSampleCube.get());
	}

	for (int i = 0;i < pBulletNum;i++)
	{
		sampleBullet[i] = std::make_unique<Object3d>();
		sampleBullet[i] = Object3d::Create(modelSampleBullet.get());
	}

	samplePlayer = std::make_unique<Object3d>();
	samplePlayer = Object3d::Create(modelSampleCube.get());

	sampleBoss = std::make_unique<Object3d>();
	sampleBoss = Object3d::Create(modelSampleCube.get());

	// カメラ注視点をセット
	camera->SetTarget({ 6.0f, 6.0f, 0 });
	camera->SetDistance(20.0f);

	//ライトの生成
	light = Light::Create();

	Audio::SoundData sound = audio->SoundLoadWave("Resources/bgm1.wav");
	//audio->SoundPlayWava(sound,true);

	// モデル名を指定してファイル読み込み
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
	XMFLOAT3 num = { 0.0f,0.0f,0.0f };

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
		camera->SetDistance(15.0f);

		// player関係変数
		pPos = { 13.5f,13.5f,-5.0f }; // playerの座標
		rot = { 0.0f,0.0f,0.0f }; // playerの回転
		qLocal = quaternion(XMFLOAT3(0, 0, 1), 0); // 姿勢用クォータニオン
		for (int i = 0;i < pBulletNum;i++)
		{
			pBullPos[i] = pPos; // playerの弾の座標
			pBulletTrigger[i] = { 0 }; // 弾の発射トリガー
			pBulletAlive[i] = { 0 }; // 弾の生存時間
		}
		speed = 0.1f; // playerの移動速度
		playerCount = 0; // 弾の発射間隔

		// enemy関係変数
		for (int i = 0;i < enemyNum;i++)
		{
			sampleEnemy[i]->SetColor({ 1,1,1,1 });
			ePos[i] = { 0.0f,0.0f,0.0f }; // mobenemyの座標
		}
		bPos = { 30.0f,30.0f,30.0f }; // bossの座標
		randNum = true; // mobenemyの座標を乱数生成
		hit = 0; // mobenemyに当たった数
		hitBoss = 0; // bossに当たった数
		sampleBoss->SetColor({ 1,1,1,1 });

		// ノイズ
		blockPos = { 0.0f,0.0f,0.0f };
		random = true;

		// シーン関係
		scene = SceneName::title;

		timer->Reset();

		if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A) || input->TriggerKey(DIK_RETURN))
		{
			scene = game;
		}

		for (int i = 0;i < enemyNum;i++)
		{
			if (randNum == true)
			{
				ePos[i].x = rand() % 30;
				ePos[i].y = rand() % 30;
				ePos[i].z = rand() % 30;
			}
		}

	}

	else if (scene == game)
	{
		//if (hitBoss > 100)
		//{
		//	scene = SceneName::end;
		//}

		if (input->TriggerKey(DIK_BACKSPACE))
		{
			scene = SceneName::end;
		}

		for (int i = 0;i < enemyNum;i++)
		{
			sampleEnemy[i]->SetScale({ 1.5f,1.5f,1.5f });
			sampleEnemy[i]->SetPosition({ ePos[i] });
		}

		sampleBoss->SetScale({ 15.0f,15.0f,15.0f });
		sampleBoss->SetPosition({ bPos });

		rot = { 0.0f,0.0f,0.0f };

		if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false) || Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
		{
			const float adjustRot = 30.0f;
			float LSticlRot = Xinput->GetPadLStickAngle() + 90.0f;
			float angle = XMConvertToRadians(LSticlRot);
			rot.x += cosf(angle) / adjustRot;
			rot.y += sinf(angle) / adjustRot;
		}

		if (input->PushKey(DIK_W)) { rot.x -= ROT_UINT; }
		if (input->PushKey(DIK_S)) { rot.x += ROT_UINT; }
		if (input->PushKey(DIK_A)) { rot.y -= ROT_UINT; }
		if (input->PushKey(DIK_D)) { rot.y += ROT_UINT; }

		if (speed > 0.2f) { speed += 0.01f; }

		XMFLOAT3 vSideAxis = getAxis(quaternion(UnitX, qLocal));
		XMFLOAT3 vUpAxis = getAxis(quaternion(UnitY, qLocal));
		XMFLOAT3 vForwardAxis = getAxis(quaternion(UnitZ, qLocal));

		Quaternion qRoll = quaternion(vUpAxis, rot.y);
		Quaternion qPitch = quaternion(vSideAxis, rot.x);
		Quaternion qYow = quaternion(vForwardAxis, rot.z);

		qLocal = qRoll * qLocal;
		qLocal = qPitch * qLocal;
		qLocal = qYow * qLocal;

		pPos.x += vForwardAxis.x * speed;
		pPos.y += vForwardAxis.y * speed;
		pPos.z += vForwardAxis.z * speed;

		playerCount++;

		for (int i = 0;i < pBulletNum;i++)
		{
			if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_RT) || input->TriggerKey(DIK_SPACE))
			{
				if (playerCount >= 10 && pBulletTrigger[i] == 0)
				{
					pBulletTrigger[i] = 1;
					pBullPos[i] = pPos;
					playerCount = 0;
				}
			}

			if (pBulletTrigger[i] == 1)
			{
				pBulletAlive[i]++;
				pBullPos[i].x += vForwardAxis.x * vec;
				pBullPos[i].y += vForwardAxis.y * vec;
				pBullPos[i].z += vForwardAxis.z * vec;
				sampleBullet[i]->SetPosition(pBullPos[i]);

				if (pBulletAlive[i] >= 50)
				{
 					pBulletTrigger[i] = 0;
					pBulletAlive[i] = 0;
				}
			}
		}

		for (int i = 0;i < enemyNum;i++)
		{
			Box mobEnemy;
			mobEnemy.center = { sampleEnemy[i]->GetPosition().x,sampleEnemy[i]->GetPosition().y,sampleEnemy[i]->GetPosition().z,1 };
			mobEnemy.scale = { sampleEnemy[i]->GetScale().x, sampleEnemy[i]->GetScale().y, sampleEnemy[i]->GetScale().z };

			Box bossEnemy;
			bossEnemy.center = { sampleBoss->GetPosition().x,sampleBoss->GetPosition().y,sampleBoss->GetPosition().z,1 };
			bossEnemy.scale = { sampleBoss->GetScale().x, sampleBoss->GetScale().y, sampleBoss->GetScale().z };

			for (int j = 0;j < pBulletNum;j++)
			{
				Sphere pBullet;
				pBullet.center = { sampleBullet[j]->GetPosition().x,sampleBullet[j]->GetPosition().y,sampleBullet[j]->GetPosition().z,1 };
				pBullet.radius = 2.0f;

				if (Collision::CheckSphere2Box(pBullet, mobEnemy))
				{
					sampleEnemy[i]->SetColor({ 1,0,0,1 });
					hit++;
				}

				//if (Collision::CheckSphere2Box(pBullet, bossEnemy))
				//{
				//	sampleBoss->SetColor({ 1,0,0,1 });
				//	hitBoss++;
				//}
			}
		}

		ImGui::Begin("Info");
		ImGui::SetWindowPos(ImVec2(20, 20), ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2(300, 200), ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
		ImGui::Text("LSticlRot %.1f", Xinput->GetPadLStickAngle());
		ImGui::Text("RSticlRot %.1f", Xinput->GetPadRStickAngle());
		ImGui::Text("cameraPos( %.1f, %.1f, %.1f )", cameraPos.x, cameraPos.y, cameraPos.z);
		ImGui::Text("num( %.5f, %.5f, %.5f )", num.x, num.y, num.z);
		ImGui::Text("hit %d", hit);
		ImGui::Text("hitBoss %d", hitBoss);
		ImGui::Text("ZE  UP/DOWN");
		ImGui::Text("AD  LEFT/RIGHT");
		ImGui::Text("SPACE  SHOT");
		ImGui::End();

		for (int i = 0;i < enemyNum;i++)
		{
			sampleEnemy[i]->Update();
		}

		sampleBoss->Update();
		sampleBoss->SetPosition(bPos);
		sampleBoss->SetScale({ 20.0f,20.0f,20.0f });
		sampleBoss->SetColor({ 1,1,1,1 });

		samplePlayer->SetPosition(pPos);
		samplePlayer->SetScale({ 0.5f,0.5f,0.5f });
		samplePlayer->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		samplePlayer->PlayerUpdate(qLocal);

		for (int i = 0;i < pBulletNum;i++)
		{
			sampleBullet[i]->SetScale({ 1.0f,1.0f,1.0f });
			sampleBullet[i]->SetColor({ 0.0f,0.0f,0.0f,1.0f });
			sampleBullet[i]->Update();
		}

		objFbx->Update();

		light->Update();

		//timer->Update();

		camera->FollowingCamera(vUpAxis, vForwardAxis, pPos);

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
		//gameBack->Draw();

		// スプライト描画後処理
		Sprite::AfterDraw();

		// 深度バッファクリア
		dxCommon->ClearDepthBuffer();

		// 3Dオブジェクトの描画前処理
		Object3d::BeforeDraw(cmdList);

		// 3Dオブジェクト描画
		if (hit < 5)
		{
			for (int i = 0;i < enemyNum;i++)
			{
				sampleEnemy[i]->Draw();
			}
		}

		else
		{
			sampleBoss->Draw();
		}

		samplePlayer->Draw();

		for (int i = 0;i < pBulletNum;i++)
		{
			if (pBulletTrigger[i] == 1)
			{
				sampleBullet[i]->Draw();
			}
		}

		// 3Dオブジェクトの描画後処理
		Object3d::AfterDraw();

		// パーティクルの描画
		//particleMan->Draw(cmdList);

		// 前景スプライト描画前処理
		Sprite::BeforeDraw(cmdList);

		//timer->Draw();
		
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