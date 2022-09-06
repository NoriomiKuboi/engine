#include "XInput.h"

//スティック制限
#define INPUT_DEADZONE  ( 0.41f * FLOAT(0x7FFF) )
//スティックの上限
const int STICK_MAX = 32767;

XInput::~XInput()
{
	//バイブレーションを止める
	EndVibration();

#pragma warning(disable:4995)
	XInputEnable(FALSE);
}

XInput* XInput::GetInstance()
{
	static XInput instance;

	return &instance;
}

void XInput::Init()
{
#pragma warning(disable:4995)
	XInputEnable(TRUE);
}

void XInput::Update()
{
	if (vibrationTimer > 0) { vibrationTimer--; }
	else if (vibrationTimer == 0)
	{
		vibrationTimer = -1;//初期化は-1
		EndVibration();
	}

	g_Controllers.lastState = g_Controllers.state;
	g_Controllers.dwResult = XInputGetState(0, &g_Controllers.state);
	if (g_Controllers.state.Gamepad.bLeftTrigger < -STICK_MAX) { g_Controllers.state.Gamepad.bLeftTrigger = -STICK_MAX; }
}

bool XInput::PushButton(XInput::PUD_BUTTON button)
{
	bool check = false;
	if (button == PUD_BUTTON::PAD_A &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_B &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_Y &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_X &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LT &&
		g_Controllers.state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RT &&
		g_Controllers.state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_START &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_BUCK &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_UP &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_DOWN &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LEFT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RIGHT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LEFT_STICK_PUSH &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RIGHT_STICK_PUSH &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		check = true;
	}
	return check;
}

bool XInput::TriggerButton(PUD_BUTTON button)
{
	bool check = false;
	if (button == PUD_BUTTON::PAD_A &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_B &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_Y &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_X &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LT &&
		g_Controllers.state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		g_Controllers.lastState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RT &&
		g_Controllers.state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		g_Controllers.lastState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_START &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_START &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_START) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_BUCK &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_UP &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_DOWN &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_LEFT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) == false)
	{
		check = true;
	}
	else if (button == PUD_BUTTON::PAD_RIGHT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) == false)
	{
		check = true;
	}
	return check;
}

bool XInput::LeftStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbLX < -INPUT_DEADZONE)
	{
		return true;
	}
	else if (LeftRight == false && g_Controllers.state.Gamepad.sThumbLX > INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::LeftStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbLY > INPUT_DEADZONE)
	{
		return true;
	}
	else if (UpDown == false && g_Controllers.state.Gamepad.sThumbLY < -INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::RightStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbRX < -INPUT_DEADZONE)
	{
		return true;
	}
	else if (LeftRight == false && g_Controllers.state.Gamepad.sThumbRX > INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::RightStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbRY > INPUT_DEADZONE)
	{
		return true;
	}
	else if (UpDown == false && g_Controllers.state.Gamepad.sThumbRY < -INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::TriggerLeftStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbLX < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLX >= -INPUT_DEADZONE)
	{
		return true;
	} if (LeftRight == false && g_Controllers.state.Gamepad.sThumbLX > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLX <= INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::TriggerLeftStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbLY > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLY <= INPUT_DEADZONE)
	{
		return true;
	}
	else if (UpDown == false && g_Controllers.state.Gamepad.sThumbLY < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLY >= -INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::TriggerRightStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbRX < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRX >= -INPUT_DEADZONE)
	{
		return true;
	}if (LeftRight == false && g_Controllers.state.Gamepad.sThumbRX > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRX <= INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

bool XInput::TriggerRightStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbRY > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRY <= INPUT_DEADZONE)
	{
		return true;
	}
	else if (UpDown == false && g_Controllers.state.Gamepad.sThumbRY < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRY >= -INPUT_DEADZONE)
	{
		return true;
	}
	else { return false; }
}

DirectX::XMFLOAT2 XInput::GetPadLStickIncline()
{
	return DirectX::XMFLOAT2((float)g_Controllers.state.Gamepad.sThumbLX / STICK_MAX, (float)g_Controllers.state.Gamepad.sThumbLY / STICK_MAX);
}

DirectX::XMFLOAT2 XInput::GetPadRStickIncline()
{
	return DirectX::XMFLOAT2((float)g_Controllers.state.Gamepad.sThumbRX / STICK_MAX, (float)g_Controllers.state.Gamepad.sThumbRY / STICK_MAX);
}

float XInput::GetPadLStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadLStickIncline();
	float h = pad.x;
	float v = pad.y;

	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	if (radian < 0)
	{
		radian += 360;
	}

	return radian;
}

float XInput::GetPadRStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadRStickIncline();
	float h = pad.x;
	float v = pad.y;

	//上向きが 0 (360)になるようにする
	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	if (radian < 0)
	{
		radian += 360;
	}

	return radian;
}

void XInput::StartVibration(STRENGTH strength, int vibrationTimer)
{
	this->vibrationTimer = vibrationTimer;

	g_Controllers.vibration.wLeftMotorSpeed = (int)strength;
	g_Controllers.vibration.wRightMotorSpeed = (int)strength;

	XInputSetState(0, &g_Controllers.vibration);
}

void XInput::EndVibration()
{
	g_Controllers.vibration.wLeftMotorSpeed = 0;
	g_Controllers.vibration.wRightMotorSpeed = 0;

	XInputSetState(0, &g_Controllers.vibration);
}
