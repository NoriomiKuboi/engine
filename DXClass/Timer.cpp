#include "Timer.h"

Timer::Timer()
{
	time = 0;
	secOne = 0;
	secTen = 0;
	count = 0;
}

Timer::~Timer()
{
	safe_delete(num0One);
	safe_delete(num1One);
	safe_delete(num2One);
	safe_delete(num3One);
	safe_delete(num0Ten);
	safe_delete(num1Ten);
	safe_delete(num2Ten);
	safe_delete(num3Ten);
	safe_delete(num4);
	safe_delete(num5);
	safe_delete(num6);
	safe_delete(num7);
	safe_delete(num8);
	safe_delete(num9);
}

void Timer::Init()
{
	if (!Sprite::LoadTexture(6, L"Resources/0.png")) // num0
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(7, L"Resources/1.png")) // num1
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(8, L"Resources/2.png")) // num2
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(9, L"Resources/3.png")) // num3
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(10, L"Resources/4.png")) // num4
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(11, L"Resources/5.png")) // num5
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(12, L"Resources/6.png")) // num6
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(13, L"Resources/7.png")) // num7
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(14, L"Resources/8.png")) // num8
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(15, L"Resources/9.png")) // num9
	{
		assert(0);
		return;
	}

	num0One = Sprite::Create(6, { 0.0f,0.0f });
	num1One = Sprite::Create(7, { 0.0f,0.0f });
	num2One = Sprite::Create(8, { 0.0f,0.0f });
	num3One = Sprite::Create(9, { 0.0f,0.0f });
	num0Ten = Sprite::Create(6, { 0.0f,0.0f });
	num1Ten = Sprite::Create(7, { 0.0f,0.0f });
	num2Ten = Sprite::Create(8, { 0.0f,0.0f });
	num3Ten = Sprite::Create(9, { 0.0f,0.0f });
	num4 = Sprite::Create(10, { 0.0f,0.0f });
	num5 = Sprite::Create(11, { 0.0f,0.0f });
	num6 = Sprite::Create(12, { 0.0f,0.0f });
	num7 = Sprite::Create(13, { 0.0f,0.0f });
	num8 = Sprite::Create(14, { 0.0f,0.0f });
	num9 = Sprite::Create(15, { 0.0f,0.0f });
}

void Timer::Update()
{
	if (secTen < 3)
	{
		time++;
	}

	if (secTen == 3)
	{
		count++;
	}

	secOne = time / 35 % 10;
	secTen = time / (35 * 10) % 6;

	//secOne
	if (secOne == 0)
	{
		num0One->SetPosition(onePos);
	}

	if (secOne == 1)
	{
		num1One->SetPosition(onePos);
	}

	if (secOne == 2)
	{
		num2One->SetPosition(onePos);
	}

	if (secOne == 3)
	{
		num3One->SetPosition(onePos);
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

	// secTen
	if (secTen == 0)
	{
		num0Ten->SetPosition(tenPos);
	}

	if (secTen == 1)
	{
		num1Ten->SetPosition(tenPos);
	}

	if (secTen == 2)
	{
		num2Ten->SetPosition(tenPos);
	}

	if (secTen == 3)
	{
		num3Ten->SetPosition(tenPos);
	}
}

void Timer::Draw()
{
	//secOne
	if (secOne == 0)
	{
		num0One->Draw();
	}

	if (secOne == 1)
	{
		num1One->Draw();
	}

	if (secOne == 2)
	{
		num2One->Draw();
	}

	if (secOne == 3)
	{
		num3One->Draw();
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

	// secTen
	if (secTen == 0)
	{
		num0Ten->Draw();
	}

	if (secTen == 1)
	{
		num1Ten->Draw();
	}

	if (secTen == 2)
	{
		num2Ten->Draw();
	}

	if (secTen == 3)
	{
		num3Ten->Draw();
	}

}

void Timer::Reset()
{
	time = 0;
	secOne = 0;
	secTen = 0;
	count = 0;
}