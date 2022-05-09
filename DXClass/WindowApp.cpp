#include "WindowApp.h"

LRESULT WindowApp::WindowProcdure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ウィンドウが破棄されたら呼ばれる
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);//OSに対してアプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理を行う
}

void WindowApp::WindowCreate()
{
	// ウィンドウクラスの設定
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProcdure; // ウィンドウプロシージャ
	winClass.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	winClass.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	RegisterClassEx(&winClass); // ウィンドウクラスをOSに登録

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, winWidth, winHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(winClass.lpszClassName, // クラス名
		L"LE2B_11_クボイノリオミ", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT, // 表示X座標（OSに任せる）
		CW_USEDEFAULT, // 表示Y座標（OSに任せる）
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		winClass.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
}

void WindowApp::WindowDelete()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}

bool WindowApp::Message()
{
	MSG msg{}; // メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがあるか
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}
