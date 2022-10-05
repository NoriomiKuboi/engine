#pragma once
class Vec2
{
public:
	float numX;
	float numY;

	Vec2();
	float VecValue(float x, float y);

	const float dot(const float x, const float y);
};