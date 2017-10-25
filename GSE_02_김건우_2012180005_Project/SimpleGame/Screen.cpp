#include "stdafx.h"
#include "Screen.h"
#include "Collision.h"

Screen::Screen() : 
	m_vec2Screen(Vector2(500, 500))
{
	float width = m_vec2Screen.x / 2;
	float height = m_vec2Screen.y / 2;

	m_pCollision[0] = Collision(Vector2(-2.0f * width, -1.0f * height), Vector2(-1.0f * width, +1.0f * height));		// 왼쪽
	m_pCollision[1] = Collision(Vector2(-1.0f * width, -2.0f * height), Vector2(+1.0f * width, -1.0f * height));		// 하단
	m_pCollision[2] = Collision(Vector2(+1.0f * width, -1.0f * height), Vector2(+2.0f * width, +1.0f * height));		// 오른쪽
	m_pCollision[3] = Collision(Vector2(-1.0f * width, +2.0f * height), Vector2(+1.0f * width, +1.0f * height - 100));	// 상단
}

Screen::Screen(int x, int y) :
	m_vec2Screen(Vector2(x, y))
{}

int Screen::CollisionOther(const Collision& Coll)
{
	for (int i = 0; i < 4; i++)
		if (m_pCollision[i].IsCollision(Coll))
			return i;

	return -1;
}
