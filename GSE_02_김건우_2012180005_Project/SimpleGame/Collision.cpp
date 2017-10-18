#include "stdafx.h"
#include "Collision.h"

bool Collision::IsCollision(Collision& other) {
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