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

	time = 0;
	secOne = 0;
	secTen = 0;
	count = 0;
}

GameScene::~GameScene()
{
	safe_delete(sprite1);
	safe_delete(titleBack);
	safe_delete(gameBack);
	safe_delete(endBack);
	safe_delete(num0One);
	safe_delete(num1One);
	safe_delete(num2One);
	safe_delete(num3One);
	safe_delete(num0Ten);
	safe_delete(num1Ten);
	safe_delete(num2Ten);
	safe_delete(num3Ten);
	safe_delete(num4);
	safe_delete(num5);
	safe_delete(num6);
	safe_delete(num7);
	safe_delete(num8);
	safe_delete(num9);
	safe_delete(particleMan);
	safe_delete(light);
	safe_delete(perlin);
	safe_delete(operation);
}

void GameScene::Init(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

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


	if (!Sprite::LoadTexture(5, L"Resources/0.png")) // num0
	{
		assert(0);
		return;
	}
	
	if (!Sprite::LoadTexture(6, L"Resources/1.png")) // num1
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(7, L"Resources/2.png")) // num2
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(8, L"Resources/3.png")) // num3
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(9, L"Resources/4.png")) // num4
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(10, L"Resources/5.png")) // num5
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(11, L"Resources/6.png")) // num6
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(12, L"Resources/7.png")) // num7
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(13, L"Resources/8.png")) // num8
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(14, L"Resources/9.png")) // num9
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(15, L"Resources/operation.png"))
	{
		assert(0);
		return;
	}

	// �w�i�X�v���C�g����
	sprite1 = Sprite::Create(1, { 0.0f,0.0f });
	titleBack = Sprite::Create(2, { 0.0f,0.0f });
	gameBack = Sprite::Create(3, { 0.0f,0.0f });
	endBack = Sprite::Create(4, { 0.0f,0.0f });
	num0One = Sprite::Create(5, { 0.0f,0.0f });
	num1One = Sprite::Create(6, { 0.0f,0.0f });
	num2One = Sprite::Create(7, { 0.0f,0.0f });
	num3One = Sprite::Create(8, { 0.0f,0.0f });
	num0Ten = Sprite::Create(5, { 0.0f,0.0f });
	num1Ten = Sprite::Create(6, { 0.0f,0.0f });
	num2Ten = Sprite::Create(7, { 0.0f,0.0f });
	num3Ten = Sprite::Create(8, { 0.0f,0.0f });
	num4 = Sprite::Create(9, { 0.0f,0.0f });
	num5 = Sprite::Create(10, { 0.0f,0.0f });
	num6 = Sprite::Create(11, { 0.0f,0.0f });
	num7 = Sprite::Create(12, { 0.0f,0.0f });
	num8 = Sprite::Create(13, { 0.0f,0.0f });
	num9 = Sprite::Create(14, { 0.0f,0.0f });
	operation = Sprite::Create(15, { 0.0f,0.0f });

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(dxCommon->GetDev(), camera);

	// ���f���ǂݍ���
	modelSampleCube = std::make_unique<Model>();
	modelSampleCube = Model::CreateFromOBJ("cubeSample");
	modelSampleBullet = std::make_unique<Model>();
	modelSampleBullet = Model::CreateFromOBJ("bulletSample");

	// 3D�I�u�W�F�N�g����
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

	// �J���������_���Z�b�g
	//camera->SetTarget({ 25.0f, 25.0f, 0 });
	//camera->SetDistance(75.0f);

	camera->SetTarget({ 6.0f, 6.0f, 0 });
	camera->SetDistance(20.0f);

	//���C�g�̐���
	light = Light::Create();

	Audio::SoundData sound = audio->SoundLoadWave("Resources/bgm1.wav");
	//audio->SoundPlayWava(sound,true);

	// ���f�������w�肵�ăt�@�C���ǂݍ���
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");
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
}

void GameScene::Update()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	const XMFLOAT3& cameraPos = camera->GetEye();

	//ImGui::Begin("Info");
	//ImGui::SetWindowPos(ImVec2(20, 20), ImGuiCond_::ImGuiCond_FirstUseEver);
	//ImGui::SetWindowSize(ImVec2(210, 110), ImGuiCond_::ImGuiCond_FirstUseEver);
	//ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	//ImGui::Text("LSticlRot %.1f", Xinput->GetPadLStickAngle());
	//ImGui::Text("RSticlRot %.1f", Xinput->GetPadRStickAngle());
	//ImGui::Text("cameraPos( %.1f, %.1f, %.1f )", cameraPos.x, cameraPos.y, cameraPos.z);
	//ImGui::End();

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
		camera->SetTarget({ 6.0f, 6.0f, 0 });
		camera->SetDistance(20.0f);

		blockPos = { 0.0f,0.0f,0.0f };
		pPos = { 6.0f,6.0f,-5.0f };
		pBullPos = pPos;
		random = true;
		trigger1 = false;
		trigger2 = false;
		vec = 0.0f;

		time = 0;
		secOne = 0;
		secTen = 0;
		count = 0;

		if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A) || input->TriggerKey(DIK_RETURN))
		{
			scene = game;
		}
	}

	else if (scene == game)
	{
		camera->Update();

		const XMFLOAT2 onePos = { 1280.0f / 2.0f,70.0f };
		const XMFLOAT2 tenPos = { 1280 / 2.0f - 48.0f,70.0f };

		if (secTen < 3)
		{
			time++;
		}

		if (secTen == 3)
		{
			count++;
		}

		if (count == 70)
		{
			scene = SceneName::end;
		}

		secOne = time / 45 % 10;
		secTen = time / (45 * 10) % 6;

		//secOne
		if (secOne == 0)
		{
			num0One->SetPosition(onePos);
		}

		if (secOne == 1)
		{
			num1One->SetPosition(onePos);
		}

		if (secOne == 2)
		{
			num2One->SetPosition(onePos);
		}

		if (secOne == 3)
		{
			num3One->SetPosition(onePos);
		}

		if (secOne == 4)
		{
			num4->SetPosition(onePos);
		}

		if (secOne == 5)
		{
			num5->SetPosition(onePos);
		}

		if (secOne == 6)
		{
			num6->SetPosition(onePos);
		}

		if (secOne == 7)
		{
			num7->SetPosition(onePos);
		}

		if (secOne == 8)
		{
			num8->SetPosition(onePos);
		}

		if (secOne == 9)
		{
			num9->SetPosition(onePos);
		}

		// secTen
		if (secTen == 0)
		{
			num0Ten->SetPosition(tenPos);
		}

		if (secTen == 1)
		{
			num1Ten->SetPosition(tenPos);
		}

		if (secTen == 2)
		{
			num2Ten->SetPosition(tenPos);
		}

		if (secTen == 3)
		{
			num3Ten->SetPosition(tenPos);
		}

		for (int x = 0; x < cubeNum; x++)
		{
			for (int y = 0; y < cubeNum; y++)
			{
				blockPos.x = float(x) * 1.5f;
				blockPos.y = float(y) * 1.5f;
				blockPos.z = perlin->Perlin(blockPos.x, blockPos.y);
				sampleBlock[x][y]->SetScale({ 0.5f,0.5f,0.5f });
				sampleBlock[x][y]->SetPosition(blockPos);
			}
		}

		if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false) || Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
		{
			float LSticlRot = Xinput->GetPadLStickAngle();
			float angle = XMConvertToRadians(LSticlRot);
			pPos.x += 0.3f * cosf(angle);
			pPos.y += 0.3f * sinf(angle);
		}

		else if (input->PushKey(DIK_W))
		{
			pPos.y += 0.3f;
		}

		else if (input->PushKey(DIK_S))
		{
			pPos.y -= 0.3f;
		}

		else if (input->PushKey(DIK_A))
		{
			pPos.x -= 0.3f;
		}

		else if (input->PushKey(DIK_D))
		{
			pPos.x += 0.3f;
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
		samplePlayer->SetScale({ 0.5f,0.5f,0.5f });
		samplePlayer->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		samplePlayer->Update();

		sampleBullet->SetPosition(pBullPos);
		sampleBullet->SetScale({ 0.5f,0.5f,0.5f });
		sampleBullet->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		sampleBullet->Update();

		objFbx->Update();

		light->Update();

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
		gameBack->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();

		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();

		// 3D�I�u�W�F�N�g�̕`��O����
		Object3d::BeforeDraw(cmdList);

		// 3D�I�u�W�F�N�g�`��
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

		// 3D�I�u�W�F�N�g�̕`��㏈��
		Object3d::AfterDraw();

		//objFbx->Draw(cmdList);
		// �p�[�e�B�N���̕`��
		//particleMan->Draw(cmdList);

		// �O�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		//secOne
		if (secOne == 0)
		{
			num0One->Draw();
		}

		if (secOne == 1)
		{
			num1One->Draw();
		}

		if (secOne == 2)
		{
			num2One->Draw();
		}

		if (secOne == 3)
		{
			num3One->Draw();
		}

		if (secOne == 4)
		{
			num4->Draw();
		}

		if (secOne == 5)
		{
			num5->Draw();
		}

		if (secOne == 6)
		{
			num6->Draw();
		}

		if (secOne == 7)
		{
			num7->Draw();
		}

		if (secOne == 8)
		{
			num8->Draw();
		}

		if (secOne == 9)
		{
			num9->Draw();
		}

		// secTen
		if (secTen == 0)
		{
			num0Ten->Draw();
		}

		if (secTen == 1)
		{
			num1Ten->Draw();
		}

		if (secTen == 2)
		{
			num2Ten->Draw();
		}

		if (secTen == 3)
		{
			num3Ten->Draw();
		}

		operation->Draw();

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

	//// �w�i�X�v���C�g�`��O����
	//Sprite::BeforeDraw(cmdList);
	//
	//// �w�i�X�v���C�g�`��
	//
	//// �X�v���C�g�`��㏈��
	//Sprite::AfterDraw();
	//
	//// �[�x�o�b�t�@�N���A
	//dxCommon->ClearDepthBuffer();
	//
	//// 3D�I�u�W�F�N�g�̕`��O����
	//Object3d::BeforeDraw(cmdList);
	//
	//// 3D�I�u�W�F�N�g�`��
	//for (int j = 0;j < cubeNum;j++)
	//{
	//	for (int i = 0;i < cubeNum;i++)
	//	{
	//		sampleBlock[j][i]->Draw();
	//	}
	//}
	//
	//samplePlayer->Draw();
	//if (trigger1 == true)
	//{
	//	sampleBullet->Draw();
	//}
	//
	//// 3D�I�u�W�F�N�g�̕`��㏈��
	//Object3d::AfterDraw();
	//
	////objFbx->Draw(cmdList);
	//// �p�[�e�B�N���̕`��
	////particleMan->Draw(cmdList);
	//
	//// �O�i�X�v���C�g�`��O����
	//Sprite::BeforeDraw(cmdList);
	//
	//// �f�o�b�O�e�L�X�g�̕`��
	//debugText.DrawAll(cmdList);
	//
	//// �X�v���C�g�`��㏈��
	//Sprite::AfterDraw();
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
