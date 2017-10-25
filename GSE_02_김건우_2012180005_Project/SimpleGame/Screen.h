#pragma once
#include "Vector2.h"
#include "Collision.h"

class Screen {
private:
	Vector2 m_vec2Screen;

	Collision m_pCollision[4];

public:
	Screen();
	Screen(int x, int y);
	~Screen() {};

	void operator=(Screen& other) { m_vec2Screen = other.m_vec2Screen; }
public:
	Vector2 GetSize() const { return m_vec2Screen; }

	void Resize() {}
	
	int CollisionOther(const Collision& Coll);
};