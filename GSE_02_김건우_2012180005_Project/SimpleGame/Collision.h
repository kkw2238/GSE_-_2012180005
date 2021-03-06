#pragma once
#include "stdafx.h"
#include "Vector2.h"
#include "Vector3.h"

class Box {
private:
	Vector3 m_vec3BegPoint;
	Vector3 m_vec3EndPoint;

public:
	Box() {};
	Box(Vector3& Vec3Beg, Vector3& Vec3End) : m_vec3BegPoint(Vec3Beg), m_vec3EndPoint(Vec3End) {}
	Box(const Vector3& vec3Mid, float size);

	Vector3 getBeg() const { return m_vec3BegPoint; }
	Vector3 getEnd() const { return m_vec3EndPoint; }
};

class Collision {
private:
	Box m_boxAABB;

public:
	Collision() {};
	Collision(Vector2& vec2Beg, Vector2& vec2End) : m_boxAABB(Vector3(vec2Beg), Vector3(vec2End)) {}
	Collision(const Vector3& vec3Mid, float size) : m_boxAABB(vec3Mid, size) {}
public:
	bool IsCollision(const Collision& other);

	void RefreshCollision(Vector2& vMid, float size);

	Box getAABB() const { return m_boxAABB; }
};