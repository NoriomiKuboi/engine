#pragma once
#include <DirectXMath.h>
#include <cmath>

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

struct Quaternion
{
	float x;// x����
	float y;// y����
	float z;// z����
	float w;// w����
};

// �N�H�[�^�j�I�����쐬
Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
Quaternion quaternion(const XMFLOAT3& v, float angle);
Quaternion quaternion(const XMFLOAT3& v, const Quaternion& q);

// ���ς����߂�
float dot(const Quaternion& q1, const Quaternion& q2);

// �m���������߂�
float length(const Quaternion& q);

// ���K������
Quaternion normalize(const Quaternion& q);

// �����l���������߂�
Quaternion conjugate(const Quaternion& q);

// �P�����Z�q�I�[�o�[���[�h
Quaternion operator + (const Quaternion& q);
Quaternion operator - (const Quaternion& q);

// ������Z�q�I�[�o�[���[�h
Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q, float s);
Quaternion& operator /= (Quaternion& q, float s);

// 2�����Z�q�I�[�o�[���[�h
Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q, float s);
Quaternion operator * (float s, const Quaternion& q);
Quaternion operator / (const Quaternion& q, float s);

Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t);
XMFLOAT3 getAxis(const Quaternion& q);
Quaternion quaternion(const XMMATRIX& m);
XMMATRIX rotate(const Quaternion& q);