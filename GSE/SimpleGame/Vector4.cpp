#include "stdafx.h"
#include "Vector4.h"

Vector4::Vector4()
{
	float x = y = z = w = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}
Vector4::Vector4(const Vector4& other)
{
	x = other.x; y = other.y; z = other.z; w = other.w;
}

Vector4::~Vector4()
{
}


