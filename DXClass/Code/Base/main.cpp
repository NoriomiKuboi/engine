#include "GameScene.h"
//#include "fbxsdk.h"
#include "FbxLoader.h"
#include "PostEffect.h"
#include "ImguiManager.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//FbxManager* fbxManager = FbxManager::Create();

	// ポインタ系
	std::unique_ptr<WindowApp> win(new WindowApp);
	std::unique_ptr<DirectXCommon> dxCommon(new DirectXCommon);
	std::unique_ptr<Input> input(new Input);
	std::unique_ptr<Audio> audio(new Audio);
	std::unique_ptr<GameScene> gameScene(new GameScene);
	std::unique_ptr<PostEffect> postEffect(new PostEffect);
	XInputManager* Xinput = nullptr;
	ImguiManager* imgui = nullptr;

	// ゲームウィンドウ作成
	win->WindowCreate();

	//　DirectX初期化
	dxCommon->Init(win.get());

	// 入力初期化
	input->Init(win->GetInstance(), win->GetHwnd());
	Xinput = XInputManager::GetInstance();
	Xinput->Init();

	// オーディオ初期化
	audio->Init();

	// スプライト初期化
	Sprite::StaticInit(dxCommon->GetDev(), WindowApp::winWidth, WindowApp::winHeight);

	// 3Dオブジェクト初期化
	Object3d::StaticInit(dxCommon->GetDev());

	// ライト初期化
	Light::StaticInit(dxCommon->GetDev());

	// FBX
	FbxLoader::GetInstance()->Init(dxCommon->GetDev());

	// ポストエフェクト用テクスチャ読み込み
	Sprite::LoadTexture(100, L"Resources/white1x1.png");
	// ポストエフェクトの初期化
	postEffect->Init();

	// ゲームシーン初期化
	gameScene->Init(dxCommon.get(), input.get(), audio.get());

	imgui = new ImguiManager(win->GetHwnd(), dxCommon->GetDev(), dxCommon->GetCmdList());

	// メインループ
	while (true)
	{
		// imgui描画前処理
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// メッセージ処理
		if (win->Message()) { break; }

		// 入力関連の更新処理
		input->Update();
		Xinput->Update();

		// ゲームシーン更新処理
		gameScene->Update();

		// レンダーテクスチャへの描画
		//postEffect->BeforeDrawScene(dxCommon->GetCmdList());
		//gameScene->Draw();
		//postEffect->AfterDrawScene(dxCommon->GetCmdList());

		// 描画開始
		dxCommon->BeforeDraw();
		// ポストエフェクトの描画
		//postEffect->Draw(dxCommon->GetCmdList());
		// ゲームシーン描画
		gameScene->Draw();
		// imgui描画
		imgui->cmdExecute();
		// 描画終了
		dxCommon->AfterDraw();

		// ESCで終了
		if (input->PushKey(DIK_ESCAPE)){ break; }
	}

	// 各種解放
	FbxLoader::GetInstance()->Final();

	// ゲームウィンドウ破棄
	win->WindowDelete();

	return 0;
}