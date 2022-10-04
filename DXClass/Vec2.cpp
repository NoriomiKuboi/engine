#include "Vec2.h"

Vec2::Vec2()
{
}

float Vec2::VecValue(float x, float y)
{
	numX = x;
	numY = y;

	return 0;
}

const float Vec2::dot(const float x, const float y)
{
	return this->numX * x + this->numY * y;
}
