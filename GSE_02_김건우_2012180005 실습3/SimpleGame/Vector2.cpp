#include "stdafx.h"
#include "Vector2.h"
#include "Utily.h"

Vector2::Vector2()
{
	float x = y = 0.0f;
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

Vector2::Vector2(const Vector2& other)
{
	x = other.x;
	y = other.y;
}

Vector2::Vector2(const Vector2* other)
{
	x = other->x; y = other->y;
}

Vector2::~Vector2()
{
}

Vector2 Vector2::Lerp(const Vector2& other, const float delta)
{
	Vector2 result = Vector2(other.x - x, other.y - y) * delta + Vector2(x, y);
	return result;
}