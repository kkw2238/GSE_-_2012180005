#include "stdafx.h"
#include "Vector3.h"
#include "Utily.h"
#include <math.h>

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

Vector3::Vector3(const Vector3* other) 
{
	x = other->x; y = other->y, z = other->z;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::Lerp(const Vector3& other, const float delta)
{
	std::cout << other << std::endl;
	Vector3 result = Vector3(other.x - x, other.y - y, other.z - z) * delta + Vector3(x, y, z);
	return result;
}

Vector3 Vector3::Normalize() {

	float u = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	return Vector3(x / u, y / u, z / u);
}