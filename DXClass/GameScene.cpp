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

	secTen = 0;
	secOne = 0;
	time = 0;
	count = 0;
}

GameScene::~GameScene()
{
	safe_delete(sprite1);
	safe_delete(titleSprite);
	safe_delete(endSprite);
	safe_delete(num0a);
	safe_delete(num1a);
	safe_delete(num2a);
	safe_delete(num3a);
	safe_delete(num4);
	safe_delete(num5);
	safe_delete(num6);
	safe_delete(num7);
	safe_delete(num8);
	safe_delete(num9);
	safe_delete(back);
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

	if (!Sprite::LoadTexture(3, L"Resources/endSample.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(4, L"Resources/num.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(5, L"Resources/0.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(6, L"Resources/1.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(7, L"Resources/2.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(8, L"Resources/3.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(9, L"Resources/4.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(10, L"Resources/5.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(11, L"Resources/6.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(12, L"Resources/7.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(13, L"Resources/8.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(14, L"Resources/9.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(15, L"Resources/back.png"))
	{
		assert(0);
		return;
	}

	// �w�i�X�v���C�g����
	sprite1 = Sprite::Create(1, { 0.0f,0.0f });
	titleSprite = Sprite::Create(2, { 0.0f,0.0f });
	endSprite = Sprite::Create(3, { 0.0f,0.0f });
	num0a = Sprite::Create(5, { 0.0f,0.0f });
	num1a = Sprite::Create(6, { 0.0f,0.0f });
	num2a = Sprite::Create(7, { 0.0f,0.0f });
	num3a = Sprite::Create(8, { 0.0f,0.0f });
	num0b = Sprite::Create(5, { 0.0f,0.0f });
	num1b = Sprite::Create(6, { 0.0f,0.0f });
	num2b = Sprite::Create(7, { 0.0f,0.0f });
	num3b = Sprite::Create(8, { 0.0f,0.0f });
	num4 = Sprite::Create(9, { 0.0f,0.0f });
	num5 = Sprite::Create(10, { 0.0f,0.0f });
	num6 = Sprite::Create(11, { 0.0f,0.0f });
	num7 = Sprite::Create(12, { 0.0f,0.0f });
	num8 = Sprite::Create(13, { 0.0f,0.0f });
	num9 = Sprite::Create(14, { 0.0f,0.0f });
	back = Sprite::Create(15, { 0.0f,0.0f });

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(dxCommon->GetDev(), camera);

	// ���f���ǂݍ���
	modelSample = Model::CreateFromOBJ("cubeSample");

	// 3D�I�u�W�F�N�g����
	for (int j = 0;j < cubeNum;j++)
	{
		for (int i = 0;i < cubeNum;i++)
		{
			objSample[j][i] = Object3d::Create(modelSample);
		}
	}

	obj = Object3d::Create(modelSample);

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
	modelFbx = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// �f�o�C�X���Z�b�g
	ObjectFbx::SetDev(dxCommon->GetDev());
	// �J�������Z�b�g
	ObjectFbx::SetCamera(camera);
	// �O���t�B�b�N�X�p�C�v���C������
	ObjectFbx::CreateGraphicsPipeline();

	// FBX�����ƃ��f���̃Z�b�g
	objFbx = new ObjectFbx;
	objFbx->Init();
	objFbx->SetModel(modelFbx);

	perlin = new Noise;
	perlin->CreateRandom(0);
}

void GameScene::Update()
{
	XInput* Xinput = XInput::GetInstance();

	// �p�[�e�B�N������
	CreateParticles();

	//��������������
	static XMVECTOR lightDir = { 0,1,10,0 };
	light->SetLightDir(lightDir);

	std::ostringstream debugstr;
	debugstr.str("");
	debugstr.clear();
	const XMFLOAT3& cameraPos = camera->GetEye();
	debugstr << "cameraPos("
		<< std::fixed << std::setprecision(2)
		<< cameraPos.x << ","
		<< cameraPos.y << ","
		<< cameraPos.z << ")";
	debugText.Print(debugstr.str(), 30, 50, 1.0f);

	camera->Update();
	particleMan->Update();

	//���C�g�̐F��ݒ�
	light->SetLightColor({ 1,1,1 });
	//3D�I�u�W�F�N�g�Ƀ��C�g��ݒ�
	Object3d::SetLight(light);

	if (scene == Scene::title)
	{
		Input::MouseMove mouseMove = input->GetMouseMove();

		pos = { 0.0f,0.0f,0.0f };
		subPos = { 0.0f,0.0f,-5.0f };
		random = true;
		trigger1 = false;
		trigger2 = false;
		vec = 0.0f;

		secTen = 0;
		secOne = 0;
		time = 0;
		count = 0;
		if (input->TriggerKey(DIK_RETURN))
		{
			scene = Scene::game;
		}
	}

	else if (scene == Scene::game)
	{
		const XMFLOAT2 tenPos = { 200.0f,100.0f };
		const XMFLOAT2 onePos = { 248.0f,100.0f };

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
			scene = Scene::end;
		}

		secTen = time / (30 * 10) % 6;
		secOne = time / 30 % 10;

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

		if (input->TriggerKey(DIK_W) && subPos.y < 12.0f)
		{
			subPos.y += 1.5f;
		}

		else if (input->TriggerKey(DIK_S) && subPos.y > 0)
		{
			subPos.y -= 1.5f;
		}

		else if (input->TriggerKey(DIK_A) && subPos.x > 0)
		{
			subPos.x -= 1.5f;
		}

		else if (input->TriggerKey(DIK_D) && subPos.x < 12.0f)
		{
			subPos.x += 1.5f;
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

		if (Xinput->TriggerButton(XInput::PUD_BUTTON::PAD_START))
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

		if (secOne == 0)
		{
			num0a->SetPosition(onePos);
		}
		if (secTen == 0)
		{
			num0b->SetPosition(tenPos);
		}
		if (secOne == 1)
		{
			num1a->SetPosition(onePos);
		}
		if (secTen == 1)
		{
			num1b->SetPosition(tenPos);
		}
		if (secOne == 2)
		{
			num2a->SetPosition(onePos);
		}
		if (secTen == 2)
		{
			num2b->SetPosition(tenPos);
		}
		if (secOne == 3)
		{
			num3a->SetPosition(onePos);
		}
		if (secTen == 3)
		{
			num3b->SetPosition(tenPos);
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

		back->SetPosition({ 0.0f,0.0f });
		obj->SetPosition(subPos);

		obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		obj->Update();

		objFbx->Update();

		light->Update();
	}

	else if (scene == Scene::end)
	{
		if (input->TriggerKey(DIK_RETURN))
		{
			scene = Scene::title;
		}
	}
}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	if (scene == Scene::title)
	{
		// �w�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �w�i�X�v���C�g�`��
		titleSprite->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();

		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
	}

	if (scene == Scene::game)
	{
		// �w�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �w�i�X�v���C�g�`��
		back->Draw();

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
				objSample[j][i]->Draw();
			}
		}

		obj->Draw();

		// 3D�I�u�W�F�N�g�̕`��㏈��
		Object3d::AfterDraw();

		// �O�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �X�v���C�g�̕`��
		if (secOne == 0)
		{
			num0a->Draw();
		}
		if (secTen == 0)
		{
			num0b->Draw();
		}
		if (secOne == 1)
		{
			num1a->Draw();
		}
		if (secTen == 1)
		{
			num1b->Draw();
		}
		if (secOne == 2)
		{
			num2a->Draw();
		}
		if (secTen == 2)
		{
			num2b->Draw();
		}
		if (secOne == 3)
		{
			num3a->Draw();
		}
		if (secTen == 3)
		{
			num3b->Draw();
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

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();
	}

	if (scene == Scene::end)
	{
		// �w�i�X�v���C�g�`��O����
		Sprite::BeforeDraw(cmdList);

		// �w�i�X�v���C�g�`��
		endSprite->Draw();

		// �X�v���C�g�`��㏈��
		Sprite::AfterDraw();

		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
	}

	// �w�i�X�v���C�g�`��O����
	Sprite::BeforeDraw(cmdList);

	// �w�i�X�v���C�g�`��

	// �X�v���C�g�`��㏈��
	Sprite::AfterDraw();

	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();

	// 3D�I�u�W�F�N�g�̕`��O����
	Object3d::BeforeDraw(cmdList);

	// 3D�I�u�W�F�N�g�`��

	// 3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::AfterDraw();

	//objFbx->Draw(cmdList);
	// �p�[�e�B�N���̕`��
	//particleMan->Draw(cmdList);

	// �O�i�X�v���C�g�`��O����
	Sprite::BeforeDraw(cmdList);

	// �f�o�b�O�e�L�X�g�̕`��
	//debugText.DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::AfterDraw();
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
