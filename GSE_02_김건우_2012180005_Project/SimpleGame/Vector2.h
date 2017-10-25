#pragma once
class Vector2
{
public:
	float x, y;

public:
	Vector2();
	Vector2(float x, float y);
	Vector2(Vector2&& other) {};
	Vector2(const Vector2& other);
	Vector2(const Vector2* other);
	~Vector2();

public:
	Vector2* GetVector2() { return this; }
	Vector2 Lerp(const Vector2& other, const float delta);

	float GetX() const { return x; }
	float GetY() const { return y; }

	void SetX(const float otherx) { x = otherx; }
	void SetY(const float othery) { y = othery; }

	void Normalize(const float min, const float max) {
		x = x > max ? max : x;
		y = y > max ? max : y;

		x = x < min ? min : x;
		y = y < min ? min : y;
	}

public:
	Vector2* operator=(const Vector2& other) {
		x = other.x; y = other.y;
		return this;
	}

	Vector2* operator+(const Vector2& other) {
		x += other.x; y += other.y;
		return this;
	}

	Vector2* operator-(const Vector2& other) {
		x -= other.x; y -= other.y;
		return this;
	}

	Vector2 operator+=(const Vector2& other) {
		x += other.x; y += other.y;
		return Vector2(x, y);
	}

	Vector2* operator-=(const Vector2& other) {
		x -= other.x; y -= other.y;
		return this;
	}

	Vector2 operator/(const float value) {
		x /= value; y /= value;
		return Vector2(x, y);
	}

	Vector2 operator*(const float value) {
		x *= value; y *= value;
		return Vector2(x, y);
	}
};

