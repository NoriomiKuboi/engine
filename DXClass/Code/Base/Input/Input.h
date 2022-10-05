#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
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
	void Init(HINSTANCE hInstance, HWND hwnd); // ������
	void Update(); // ���t���[������
	bool PushKey(BYTE keyNumber); // �L�[�̉������`�F�b�N
	bool TriggerKey(BYTE keyNumber); // �L�[�̃g���K�[���`�F�b�N
	bool PushMouseLeft(); // �L�[�̍��{�^���������`�F�b�N
	bool PushMouseMiddle(); // �L�[�̒��{�^���������`�F�b�N
	bool TriggerMouseLeft(); // �L�[�̍��{�^���g���K�[���`�F�b�N
	bool TriggerMouseMiddle(); // �L�[�̒��{�^���g���K�[���`�F�b�N
	MouseMove GetMouseMove(); // �}�E�X�ړ��ʂ��擾

private:
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
};

