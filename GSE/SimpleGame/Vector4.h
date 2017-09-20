#pragma once
class Vector4
{
private:
	float x, y, z, w;

public:
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector4&& other) {};
	Vector4(const Vector4& other);
	~Vector4();

public:
	Vector4* GetVector4() { return this; }

	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetZ() const { return z; }
	float GetW() const { return w; }

	void SetX(const float otherx) { x = otherx; }
	void SetY(const float othery) { y = othery; }
	void SetZ(const float otherz) { z = otherz; }

	void Normalize(const float min, const float max) {
		x = x > max ? max : x;
		y = y > max ? max : y;
		z = z > max ? max : z;
		w = w > max ? max : w;

		x = (x < min ? min : x) / max;
		y = (y < min ? min : y) / max;
		z = (z < min ? min : z) / max;
		w = (w < min ? min : w) / max;
	}

public:
	Vector4* operator=(Vector4& other) {
		x = other.x; y = other.y; z = other.z; w = other.w;
		return this;
	}

	Vector4* operator+(Vector4& other) {
		x += other.x; y += other.y; z += other.z; w += other.w;
		return this;
	}

	Vector4* operator-(Vector4& other) {
		x -= other.x; y -= other.y; z -= other.z; w -= other.w;
		return this;
	}

	Vector4* operator+=(Vector4& other) {
		x += other.x; y += other.y; z += other.z; w += other.w;
		return this;
	}

	Vector4* operator-=(Vector4& other) {
		x -= other.x; y -= other.y; z -= other.z; w -= other.w;
		return this;
	}
};
