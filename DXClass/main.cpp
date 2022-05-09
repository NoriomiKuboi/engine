#include "GameScene.h"
//#include "fbxsdk.h"
#include "FbxLoader.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//FbxManager* fbxManager = FbxManager::Create();

	// ポインタ系
	WindowApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウ作成
	win = new WindowApp();
	win->WindowCreate();

	//　DirectX初期化
	dxCommon = new DirectXCommon();
	dxCommon->Init(win);

	// 入力初期化
	input = new Input();
	input->Init(win->GetInstance(), win->GetHwnd());

	// オーディオ初期化
	audio = new Audio();
	audio->Init();

	// スプライト初期化
	Sprite::StaticInit(dxCommon->GetDev(), WindowApp::winWidth, WindowApp::winHeight);

	// 3Dオブジェクト初期化
	Object3d::StaticInit(dxCommon->GetDev());

	// ライト初期化
	Light::StaticInit(dxCommon->GetDev());

	// FBX
	FbxLoader::GetInstance()->Init(dxCommon->GetDev());

	// ゲームシーン初期化
	gameScene = new GameScene();
	gameScene->Init(dxCommon, input, audio);

	// メインループ
	while (true)
	{
		// メッセージ処理
		if (win->Message()) { break; }

		// 入力関連の更新処理
		input->Update();

		// ゲームシーン更新処理
		gameScene->Update();

		// 描画開始
		dxCommon->BeforeDraw();
		// ゲームシーン描画
		gameScene->Draw();
		// 描画終了
		dxCommon->AfterDraw();

		// ESCで終了
		if (input->PushKey(DIK_ESCAPE)){ break; }
	}

	// 各種解放
	safe_delete(gameScene);
	safe_delete(audio);
	safe_delete(input);
	safe_delete(dxCommon);
	FbxLoader::GetInstance()->Final();

	// ゲームウィンドウ破棄
	win->WindowDelete();
	safe_delete(win);

	return 0;
}