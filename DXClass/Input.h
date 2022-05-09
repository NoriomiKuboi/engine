#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

class Input
{
public:
	struct MouseMove
	{
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Init(HINSTANCE hInstance, HWND hwnd); // 初期化
	void Update(); // 毎フレーム処理
	bool PushKey(BYTE keyNumber); // キーの押下をチェック
	bool TriggerKey(BYTE keyNumber); // キーのトリガーをチェック
	bool PushMouseLeft(); // キーの左ボタン押下をチェック
	bool PushMouseMiddle(); // キーの中ボタン押下をチェック
	bool TriggerMouseLeft(); // キーの左ボタントリガーをチェック
	bool TriggerMouseMiddle(); // キーの中ボタントリガーをチェック
	MouseMove GetMouseMove(); // マウス移動量を取得

private:
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
};

