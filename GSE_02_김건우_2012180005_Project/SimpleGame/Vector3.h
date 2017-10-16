#pragma once
class Vector3
{
private:
	float x, y, z;

public:
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(Vector3&& other) {};
	Vector3(const Vector3& other);
	Vector3(const Vector3* other);
	~Vector3();

public:
	Vector3* GetVector3() { return this; }
	Vector3 Lerp(const Vector3& other, const float delta);
	Vector3 Normalize();

	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetZ() const { return z; }

	void SetX(const float otherx) { x = otherx; }
	void SetY(const float othery) { y = othery; }
	void SetZ(const float otherz) { z = otherz; }

	void MinMax( const float min , const float max) {
		x = x > max ? max : x;
		y = y > max ? max : y;
		z = z > max ? max : z;

		x = x < min ? min : x;
		y = y < min ? min : y;
		z = z < min ? min : z;
	}

public:
	Vector3* operator=(const Vector3& other) {
		x = other.x; y = other.y; z = other.z;
		return this;
	}

	Vector3* operator+(const Vector3& other) {
		x += other.x; y += other.y; z += other.z;
		return this;
	}

	Vector3* operator-(const Vector3& other) {
		x -= other.x; y -= other.y; z -= other.z;
		return this;
	}

	Vector3 operator+=(const Vector3& other) {
		x += other.x; y += other.y; z += other.z;
		return Vector3(x, y, z);
	}

	Vector3* operator-=(const Vector3& other) {
		x -= other.x; y -= other.y; z -= other.z;
		return this;
	}

	Vector3 operator/(const float value) {
		x /= value; y /= value; z /= value;
		return Vector3(x, y, z);
	}

	Vector3 operator*(const float value) {
		x *= value; y *= value; z *= value;
		return Vector3(x, y, z);
	}
};

