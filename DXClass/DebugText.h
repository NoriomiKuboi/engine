#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

class DebugText
{
public:
	static const int maxCharCount = 256; // �ő啶����
	static const int fontWidth = 9; // �t�H���g�摜��1�������̉���
	static const int fontHeight = 18; // �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14; // �t�H���g�摜��1�s���̕�����

	DebugText();
	~DebugText();

	void Init(UINT texnumber);

	void Print(const std::string& text, float x, float y, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	Sprite* spriteDatas[maxCharCount] = {}; // �X�v���C�g�f�[�^�̔z��
	int spriteIndex = 0; // �X�v���C�g�f�[�^�z��̓Y�����ԍ�
};
