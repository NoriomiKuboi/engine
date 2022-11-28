#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include "ObjectFbx.h"
#include <iostream>

using namespace DirectX;

GameScene::GameScene()
{
	// player�֌W�ϐ�
	pPos = { 13.5f,13.5f,-5.0f }; // player�̍��W
	rot = { 0.0f,0.0f,0.0f }; // player�̉�]
	qLocal = quaternion(XMFLOAT3(0, 0, 1), 0); // �p���p�N�H�[�^�j�I��
	for (int i = 0;i < pBulletNum;i++)
	{
		pBullPos[i] = pPos; // player�̒e�̍��W
		pBulletTrigger[i] = { 0 }; // �e�̔��˃g���K�[
		pBulletAlive[i] = 0;  // �e�̐�������
	}
	speed = 0.1f; // player�̈ړ����x
	playerCount = 0; // �e�̔��ˊԊu

	// enemy�֌W�ϐ�
	for (int i = 0;i < enemyNum;i++)
	{
		ePos[i] = { 0.0f,0.0f,0.0f }; // mobenemy�̍��W

		pos[i] = { 0.0f,0.0f,0.0f };
	}
	bPos = { 30.0f,30.0f,30.0f }; // boss�̍��W
	randNum = true; // mobenemy�̍��W�𗐐�����
	hit = 0; // mobenemy�ɓ���������
	hitBoss = 0; // boss�ɓ���������

	// �m�C�Y
	blockPos = { 0.0f,0.0f,0.0f };
	random = true;

	// �V�[���֌W
	scene = SceneName::title;

	reticlePos = { 1280 / 2 - 32, 720 / 2 - 32 };

	move = { 0.0f,0.0f,0.0f };

	count = 0;

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
	safe_delete(reticle);
	safe_delete(timer);
}

void GameScene::Init(WindowApp* win, DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(win);
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->win = win;
	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
		return;
	}

	// �f�o�b�O�e�L�X�g������
	debugText.Init(debugTextTexNumber);
	
	// ����������
	//srand(time(NULL));

	// �J��������
	camera = new CameraSub(WindowApp::winWidth, WindowApp::winHeight, input);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);

	// �e�N�X�`���ǂݍ���
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

	if (!Sprite::LoadTexture(16, L"Resources/reticle.png"))
	{
		assert(0);
		return;
	}

	// �w�i�X�v���C�g����
	sprite1 = Sprite::Create(1, { 0.0f,0.0f });
	titleBack = Sprite::Create(2, { 0.0f,0.0f });
	gameBack = Sprite::Create(3, { 0.0f,0.0f });
	endBack = Sprite::Create(4, { 0.0f,0.0f });
	operation = Sprite::Create(5, { 0.0f,0.0f });
	reticle = Sprite::Create(16, { 0.0f,0.0f });

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(dxCommon->GetDev(), camera);

	// ���f���ǂݍ���
	modelSampleCube = std::make_unique<Model>();
	modelSampleCube = Model::CreateFromOBJ("cubeSample");
	modelSampleBullet = std::make_unique<Model>();
	modelSampleBullet = Model::CreateFromOBJ("bulletSample");

	// 3D�I�u�W�F�N�g����
	for (int i = 0;i < enemyNum;i++)
	{
		sampleEnemy[i] = std::make_unique<Object3d>();
		sampleEnemy[i] = Object3d::Create(modelSampleCube.get());
		sampleBulletEnemy[i] = std::make_unique<Object3d>();
		sampleBulletEnemy[i] = Object3d::Create(modelSampleCube.get());
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

	// �J���������_���Z�b�g
	camera->SetTarget({ 6.0f, 6.0f, 0 });
	camera->SetDistance(20.0f);

	//���C�g�̐���
	light = Light::Create();

	Audio::SoundData sound = audio->SoundLoadWave("Resources/bgm1.wav");
	//audio->SoundPlayWava(sound,true);

	// ���f�������w�肵�ăt�@�C���ǂݍ���
	modelFbx = std::make_unique<Model>();
	modelFbx = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// �f�o�C�X���Z�b�g
	ObjectFbx::SetDev(dxCommon->GetDev());
	// �J�������Z�b�g
	ObjectFbx::SetCamera(camera);
	// �O���t�B�b�N�X�p�C�v���C������
	ObjectFbx::CreateGraphicsPipeline();

	// FBX�����ƃ��f���̃Z�b�g
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

	// �p�[�e�B�N������
	CreateParticles();

	//��������������
	static XMVECTOR lightDir = { 0,1,10,0 };
	light->SetLightDir(lightDir);

	particleMan->Update();

	//���C�g�̐F��ݒ�
	light->SetLightColor({ 1,1,1 });

	//3D�I�u�W�F�N�g�Ƀ��C�g��ݒ�
	Object3d::SetLight(light);

	if (scene == title)
	{
		camera->SetTarget({ 6.5f, 6.5f, 0 });
		camera->SetDistance(15.0f);

		// player�֌W�ϐ�
		pPos = { 13.5f,13.5f,-5.0f }; // player�̍��W
		rot = { 0.0f,0.0f,0.0f }; // player�̉�]
		qLocal = quaternion(XMFLOAT3(0, 0, 1), 0); // �p���p�N�H�[�^�j�I��
		for (int i = 0;i < pBulletNum;i++)
		{
			pBullPos[i] = pPos; // player�̒e�̍��W
			pBulletTrigger[i] = { 0 }; // �e�̔��˃g���K�[
			pBulletAlive[i] = { 0 }; // �e�̐�������
		}
		speed = 0.1f; // player�̈ړ����x
		playerCount = 0; // �e�̔��ˊԊu

		// enemy�֌W�ϐ�
		for (int i = 0;i < enemyNum;i++)
		{
			sampleEnemy[i]->SetColor({ 1,1,1,1 });
			ePos[i] = { 0.0f,0.0f,0.0f }; // mobenemy�̍��W
			pos[i] = ePos[i];
		}
		bPos = { 30.0f,30.0f,30.0f }; // boss�̍��W
		randNum = true; // mobenemy�̍��W�𗐐�����
		hit = 0; // mobenemy�ɓ���������
		hitBoss = 0; // boss�ɓ���������
		sampleBoss->SetColor({ 1,1,1,1 });

		// �m�C�Y
		blockPos = { 0.0f,0.0f,0.0f };
		random = true;

		// �V�[���֌W
		scene = SceneName::title;

		reticlePos = { 1280 / 2 - 32, 720 / 2 - 32 };

		timer->Reset();

		move = { 0.0f,0.0f,0.0f };

		count = 0;

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
		if (hitBoss > 1)
		{
			scene = SceneName::end;
		}

		if (input->TriggerKey(DIK_BACKSPACE))
		{
			scene = SceneName::end;
		}

		ImGui::Begin("Info");
		ImGui::SetWindowPos(ImVec2(20, 20), ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2(300, 200), ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		//ImGui::Text("LSticlRot %.1f", Xinput->GetPadLStickAngle());
		//ImGui::Text("RSticlRot %.1f", Xinput->GetPadRStickAngle());
		//ImGui::Text("cameraPos( %.1f, %.1f, %.1f )", cameraPos.x, cameraPos.y, cameraPos.z);
		//ImGui::Text("ScreenPos( %.1d, %.1d )", mousePos.x, mousePos.y);
		//ImGui::Text("PosNear( %.1f, %.1f, %.1f )", posNear.x, posNear.y, posNear.z);
		//ImGui::Text("PosFar( %.1f, %.1f, %.1f )", posFar.x, posFar.y, posFar.z);
		ImGui::Text("hit %d", hit);
		ImGui::Text("hitBoss %d", hitBoss);
		//ImGui::Text("reticlePos( %.1f, %.1f )", reticlePos.x, reticlePos.y);
		//ImGui::Text("translation( %.1f, %.1f, %.1f )", translation.x, translation.y, translation.z);
		ImGui::Text("move( %.2f, %.2f, %.2f )", move.x, move.y, move.z);
		ImGui::Text("count %d", count);
		ImGui::End();

		count++;

		const float num = 0.05f;

		if (count < 50) {
			move.x += num;
		}

		if (count >= 50) {
			move.x -= num;
		}

		if (count > 100) {
			count = 0;
		}


		for (int i = 0;i < enemyNum;i++) {
			if (count == 0) {
				pos[i] = ePos[i];
			}

			if (count < 100) {
				pos[i].z -= 1.0f;
			}

			sampleEnemy[i]->SetScale({ 1.0f,1.0f,1.0f });
			sampleEnemy[i]->SetPosition({ ePos[i].x + move.x, ePos[i].y + move.y , ePos[i].z + move.z });
			sampleBulletEnemy[i]->SetScale({ 0.3f,0.3f,0.3f });
			sampleBulletEnemy[i]->SetPosition({ pos[i] });
		}

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

			Sphere pBullet;
			pBullet.center = { sampleBullet[i]->GetPosition().x,sampleBullet[i]->GetPosition().y,sampleBullet[i]->GetPosition().z,1 };
			pBullet.radius = 2.0f;
			
			for (int j = 0;j < enemyNum;j++)
			{
				Box mobEnemy;
				mobEnemy.center = { sampleEnemy[j]->GetPosition().x,sampleEnemy[j]->GetPosition().y,sampleEnemy[j]->GetPosition().z,1 };
				mobEnemy.scale = { sampleEnemy[j]->GetScale().x, sampleEnemy[j]->GetScale().y, sampleEnemy[j]->GetScale().z };

				Box enemyBullet;
				enemyBullet.center = { sampleBulletEnemy[j]->GetPosition().x,sampleBulletEnemy[j]->GetPosition().y,sampleBulletEnemy[j]->GetPosition().z,1 };
				enemyBullet.scale = { sampleBulletEnemy[j]->GetScale().x, sampleBulletEnemy[j]->GetScale().y, sampleBulletEnemy[j]->GetScale().z };
			
				Box bossEnemy;
				bossEnemy.center = { sampleBoss->GetPosition().x,sampleBoss->GetPosition().y,sampleBoss->GetPosition().z,1 };
				bossEnemy.scale = { sampleBoss->GetScale().x, sampleBoss->GetScale().y, sampleBoss->GetScale().z };
			
				if (Collision::CheckSphere2Box(pBullet, mobEnemy))
				{
					sampleEnemy[j]->SetColor({ 1,0,0,1 });
					hit++;
				}

				if (Collision::CheckSphere2Box(pBullet, enemyBullet))
				{
					samplePlayer->SetColor({ 0,0,1,1 });
				}
			
				if (hit > 5 && Collision::CheckSphere2Box(pBullet, bossEnemy))
				{
					sampleBoss->SetColor({ 1,0,0,1 });
					hitBoss++;
				}
			}
		}

		//POINT mousePos;
		//GetCursorPos(&mousePos);

		for (int i = 0;i < enemyNum;i++)
		{
			sampleEnemy[i]->Update();
			sampleBulletEnemy[i]->Update();
		}

		sampleBoss->SetPosition(bPos);
		sampleBoss->SetScale({ 10.0f,10.0f,10.0f });
		sampleBoss->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		sampleBoss->Update();

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

		Reticle();
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
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	if (scene == title)
	{
		// �w�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �w�i�X�v���C�g�`��
		titleBack->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();

		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
	}

	else if (scene == game)
	{
		// �w�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �w�i�X�v���C�g�`��
		//gameBack->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();

		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();

		// 3D�I�u�W�F�N�g�̕`��O����
		Object3d::BeforeDraw(cmdList);

		// 3D�I�u�W�F�N�g�`��
		if (hit <= 5)
		{
			for (int i = 0;i < enemyNum;i++)
			{
				sampleEnemy[i]->Draw();
				sampleBulletEnemy[i]->Draw();
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

		// 3D�I�u�W�F�N�g�̕`��㏈��
		Object3d::AfterDraw();

		// �p�[�e�B�N���̕`��
		//particleMan->Draw(cmdList);

		// �O�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		//timer->Draw();
		
		operation->Draw();
		//reticle->Draw();

		// �f�o�b�O�e�L�X�g�̕`��
		debugText.DrawAll(cmdList);

		// �O�i�X�v���C�g�`��㏈��
		Sprite::AfterDraw();
	}

	else if (scene == end)
	{
		// �w�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �w�i�X�v���C�g�`��
		endBack->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();

		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
	}
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++)
	{
		// X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
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
		//�F�������_���ɕ��z
		XMFLOAT4 color;
		color.w = 1;
		color.x = (float)rand() / RAND_MAX * 1;
		color.y = (float)rand() / RAND_MAX * 1;
		color.z = (float)rand() / RAND_MAX * 1;

		// �ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}

void GameScene::Reticle()
{
	// �}�E�X�̍��W�擾
	POINT mousePos;
	GetCursorPos(&mousePos);

	// �N���C�A���g�G���A���W�ɕϊ�
	HWND hwnd = win->GetHwnd();
	ScreenToClient(hwnd, &mousePos);

	// �X�v���C�g���W�ɑ��
	reticlePos.x = static_cast<float> (mousePos.x) - 32.0f;
	reticlePos.y = static_cast<float> (mousePos.y) - 32.0f;
	reticle->SetPosition(reticlePos);

	// �r���[�v���W�F�N�V�����r���[�|�[�g�s�񍇐�
	XMMATRIX matView, matProjection, matViewPort;
	matView = camera->GetViewMatrix();
	matProjection = camera->GetProjectionMatrix();
	matViewPort = {
		WindowApp::winWidth / 2, 0, 0, 0,
		0,-WindowApp::winHeight / 2, 0, 0,
		0,0, 1, 0,
		WindowApp::winWidth / 2, WindowApp::winHeight / 2, 0, 1
	};
	XMMATRIX matVPV = matView * matProjection * matViewPort;

	// �����s��̋t�s����v�Z����
	XMMATRIX matInverseVPV = XMMatrixInverse(nullptr, matVPV);

	// �X�N���[�����W
	XMFLOAT3 posNear = XMFLOAT3(mousePos.x, mousePos.y, 0);
	XMFLOAT3 posFar = XMFLOAT3(mousePos.x, mousePos.y, 1);

	// �X�N���[�����W�n���烏�[���h���W��
	XMVector3TransformCoordStream(&posNear, sizeof(XMFLOAT3), &posNear, sizeof(XMFLOAT3), 1, matInverseVPV);
	XMVector3TransformCoordStream(&posFar, sizeof(XMFLOAT3), &posFar, sizeof(XMFLOAT3), 1, matInverseVPV);

	//�@�}�E�X���C�̕���
	XMFLOAT3 mouseDirection = { posFar.x - posNear.x, posFar.y - posNear.y ,posFar.z - posNear.z };
	float mouseDirectionLength = sqrtf(pow(mouseDirection.x, 2) + pow(mouseDirection.y, 2) + pow(mouseDirection.z, 2));
	float mouseDirectionNormalize = 1 / mouseDirectionLength;

	// �J��������W���I�u�W�F�N�g�̋���
	const float kDistanceTestObject = 20.0f;
	XMFLOAT3 translation = { posNear.x * mouseDirectionNormalize, posNear.y * mouseDirectionNormalize, posNear.z * mouseDirectionNormalize };
	for (int i = 0;i < pBulletNum;i++)
	{
		//sampleBullet[i]->SetPosition(translation);
	}



	// �R���g���[���[�̏���
	//XInputManager* Xinput = XInputManager::GetInstance();
	//if (Xinput->RightStickX(true) || Xinput->RightStickX(false) || Xinput->RightStickY(true) || Xinput->RightStickY(false))
	//{
	//	float RSticlRot = Xinput->GetPadRStickAngle();
	//	float angle = XMConvertToRadians(RSticlRot);
	//	reticlePos.x += cosf(angle) * 10.0f;
	//	reticlePos.y -= sinf(angle) * 10.0f;
	//}
	//
	//reticle->SetPosition(reticlePos);
}