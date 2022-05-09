#pragma once
#include <Windows.h>

class WindowApp
{
public:
	static const int winWidth = 1280; // ����
	static const int winHeight = 720; // �c��

public:
	static LRESULT WindowProcdure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); // �E�B���h�E�v���V�[�W��

public:
	void WindowCreate(); // �Q�[���E�B���h�E�̍쐬
	void WindowDelete(); // �Q�[���E�B���h�E�̔j��
	bool Message(); // ���b�Z�[�W�̏���

	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetInstance() { return winClass.hInstance; }

private:
	HWND hwnd = nullptr;
	WNDCLASSEX winClass{}; // �E�B���h�E�N���X�̐ݒ�
};