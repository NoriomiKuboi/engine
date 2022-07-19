#pragma once
#include "Object3d.h"

#include <DirectXMath.h>

class Noise
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Noise(); // �R���X�g���N�^
	float Perlin(float x, float y); // �m�C�Y�v�Z
	void CreateRandom(unsigned int seed); // �����e�[�u���̐���

private:
	// �ϐ�
	static const int splitNum = 10; // ������
	static const int maxNum = 256; // �ő吔
	static const int tableNum = maxNum * 2; // �����e�[�u���̍ő�l
	int numCode[tableNum];

	// �֐�
	float Grad(int hash, float x, float y); // �O���f�B�G���g�֐�(���z���v�Z)
	float Fade(float t); // �t�F�[�h�֐�
	float Lerp(float a, float b, float x); // ���`���
	int RandomGet(int x, int y); // �����̎擾
};