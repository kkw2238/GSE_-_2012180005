#include "stdafx.h"
#include "Collision.h"

Box::Box(const Vector3& vec3Mid, float size) : 
	m_vec3BegPoint(Vector2(vec3Mid.x - size / 2, vec3Mid.y - size / 2)),
	m_vec3EndPoint(Vector2(vec3Mid.x + size / 2, vec3Mid.y + size / 2))
{}

bool Collision::IsCollision(const Collision& other) {
	Vector3 memberBeg = m_boxAABB.getBeg();
	Vector3 memberEnd = m_boxAABB.getEnd();

	Vector3 otherBeg = other.getAABB().getBeg();
	Vector3 otherEnd = other.getAABB().getEnd();

	if (memberBeg.x >= otherEnd.x) return false;
	if (memberBeg.y >= otherEnd.y) return false;
	if (memberEnd.x <= otherBeg.x) return false;
	if (memberEnd.y <= otherBeg.y) return false;

	return true;
}

void Collision::RefreshCollision(Vector2& vMid, float size)
{
	m_boxAABB = Box(vMid, size);
}