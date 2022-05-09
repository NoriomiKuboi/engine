#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

class DebugText
{
public:
	static const int maxCharCount = 256; // 最大文字数
	static const int fontWidth = 9; // フォント画像内1文字分の横幅
	static const int fontHeight = 18; // フォント画像内1文字分の縦幅
	static const int fontLineCount = 14; // フォント画像内1行分の文字数

	DebugText();
	~DebugText();

	void Init(UINT texnumber);

	void Print(const std::string& text, float x, float y, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	Sprite* spriteDatas[maxCharCount] = {}; // スプライトデータの配列
	int spriteIndex = 0; // スプライトデータ配列の添え字番号
};
