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
	scene = SceneName::title;

	sec = 0;
	time = 0;
	count = 0;

	speed = 0.1f;
	rot = { 0.0f,0.0f,0.0f };
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

	if (!Sprite::LoadTexture(5, L"Resources/operation.png"))
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

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(dxCommon->GetDev(), camera);

	// ���f���ǂݍ���
	modelSampleCube = std::make_unique<Model>();
	modelSampleCube = Model::CreateFromOBJ("cubeSample");
	modelSampleBullet = std::make_unique<Model>();
	modelSampleBullet = Model::CreateFromOBJ("bulletSample");

	// 3D�I�u�W�F�N�g����
	for (int k = 0;k < cubeNum;k++)
	{
		for (int j = 0;j < cubeNum;j++)
		{
			for (int i = 0;i < cubeNum;i++)
			{
				sampleBlock[k][j][i] = std::make_unique<Object3d>();
				sampleBlock[k][j][i] = Object3d::Create(modelSampleCube.get());
			}
		}
	}

	samplePlayer = std::make_unique<Object3d>();
	samplePlayer = Object3d::Create(modelSampleCube.get());
	sampleBullet = std::make_unique<Object3d>();
	sampleBullet = Object3d::Create(modelSampleBullet.get());

	// �J���������_���Z�b�g
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

	timer = new Timer;
	timer->Init();
}

void GameScene::Update()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	const XMFLOAT3& cameraPos = camera->GetEye();
	XMFLOAT3 num = { 0.0f,0.0f,0.0f };

	ImGui::Begin("Info");
	ImGui::SetWindowPos(ImVec2(20, 20), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(300, 200), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::Text("LSticlRot %.1f", Xinput->GetPadLStickAngle());
	ImGui::Text("RSticlRot %.1f", Xinput->GetPadRStickAngle());
	ImGui::Text("cameraPos( %.1f, %.1f, %.1f )", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("num( %.5f, %.5f, %.5f )", num.x, num.y, num.z);
	ImGui::Text("ZE  UP/DOWN");
	ImGui::Text("AD  LEFT/RIGHT");
	ImGui::Text("SPACE  SHOT");
	ImGui::End();

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
		camera->SetDistance(20.0f);

		blockPos = { 0.0f,0.0f,0.0f };
		pPos = { 6.5f,6.5f,-5.0f };
		rot = { 0.0f,0.0f,0.0f };
		qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);
		pBullPos = pPos;
		random = true;
		trigger1 = false;
		trigger2 = false;

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

		sec = time / (35 * 10) % 6;

		for (int x = 0; x < cubeNum; x++)
		{
			for (int y = 0; y < cubeNum; y++)
			{
				for (int z = 0;z < cubeNum;z++)
				{
					blockPos.x = float(x) * 3.0f;
					blockPos.y = float(y) * 3.0f;
					blockPos.z = float(z) * 3.0f;
					//blockPos.z = perlin->Perlin(blockPos.x, blockPos.y);
					//sampleBlock[x][y]->SetScale({ 0.8f,0.8f,0.8f });
					sampleBlock[x][y][z]->SetScale({ 0.5f,0.5f,0.5f });
					sampleBlock[x][y][z]->SetPosition(blockPos);
				}
			}
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

		if (input->PushKey(DIK_E)) { rot.x -= ROT_UINT; }
		if (input->PushKey(DIK_Z)) { rot.x += ROT_UINT; }
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

		for (int x = 0;x < cubeNum;x++)
		{
			for (int y = 0;y < cubeNum;y++)
			{
				for (int z = 0;z < cubeNum;z++)
				{
					sampleBlock[x][y][z]->Update();
				}
			}
		}

		if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_RT) || input->TriggerKey(DIK_SPACE))
		{
			trigger1 = true;
			pBullPos = pPos;
		}

		if (trigger1 == true)
		{
			pBullPos.x += vForwardAxis.x * 2.0f;
			pBullPos.y += vForwardAxis.y * 2.0f;
			pBullPos.z += vForwardAxis.z * 2.0f;
		}

		if (pBullPos.z > 20.0f)
		{
			pBullPos = pPos;
			trigger1 = false;
			trigger2 = false;
		}

		for (int x = 0;x < cubeNum;x++)
		{
			for (int y = 0;y < cubeNum;y++)
			{
				for (int z = 0;z < cubeNum;z++)
				{
					Sphere pBullet;
					pBullet.center = { sampleBlock[x][y][z]->GetPosition().x,sampleBlock[x][y][z]->GetPosition().y,sampleBlock[x][y][z]->GetPosition().z,1 };
					pBullet.radius = 1;
					Box block;
					block.center = { sampleBullet->GetPosition().x,sampleBullet->GetPosition().y,sampleBullet->GetPosition().z,1 };

					if (Collision::CheckSphere2Box(pBullet, block))
					{
						sampleBlock[x][y][z]->SetColor({ 1,0,0,1 });
					}

					else
					{
						sampleBlock[x][y][z]->SetColor({ 0,0.4f,0,1 });
					}
				}
			}
		}

		samplePlayer->SetPosition(pPos);
		samplePlayer->SetScale({ 0.5f,0.5f,0.5f });
		samplePlayer->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//samplePlayer->Update();
		samplePlayer->PlayerUpdate(qLocal);
		camera->FollowingCamera(vUpAxis, vForwardAxis, pPos);

		sampleBullet->SetPosition(pBullPos);
		sampleBullet->SetScale({ 0.5f,0.5f,0.5f });
		sampleBullet->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		sampleBullet->Update();

		objFbx->Update();

		light->Update();

		timer->Update();

		//camera->Update();
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
		for (int k = 0;k < cubeNum;k++)
		{
			for (int j = 0;j < cubeNum;j++)
			{
				for (int i = 0;i < cubeNum;i++)
				{
					sampleBlock[k][j][i]->Draw();
				}
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

		timer->Draw();
		
		//operation->Draw();

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