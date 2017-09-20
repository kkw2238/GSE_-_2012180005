#include "stdafx.h"
#include "Vector3.h"


Vector3::Vector3()
{
	float x = y = z = 0.0f;
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector3::Vector3(const Vector3& other) 
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::Lerp(const Vector3& other, const float delta) 
{
	Vector3 result(Vector3(other.x - x, other.y - y, other.z - z) * delta);

	return result;
}