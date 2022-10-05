#pragma once
#include <Windows.h>

class WindowApp
{
public:
	static const int winWidth = 1280; // 横幅
	static const int winHeight = 720; // 縦幅

public:
	static LRESULT WindowProcdure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); // ウィンドウプロシージャ

public:
	void WindowCreate(); // ゲームウィンドウの作成
	void WindowDelete(); // ゲームウィンドウの破棄
	bool Message(); // メッセージの処理

	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetInstance() { return winClass.hInstance; }

private:
	HWND hwnd = nullptr;
	WNDCLASSEX winClass{}; // ウィンドウクラスの設定
};