#include "stdafx.h"
#include "ObjectManagement.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Utily.h"


Object::Object()
{
}

Object::Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend) :
	m_vPos(pos),
	m_fSize(size),
	m_vColor(color),
	m_pRenderer(rend),
	m_vDirection(Vector3()),
	m_fValocity(10.0f)
{}

Object::Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend) :
	m_vPos(Vector3(x, y, z)),
	m_fSize(size),
	m_vColor(Vector4(r, g, b, a)),
	m_pRenderer(rend),
	m_vDirection(Vector3()),
	m_fValocity(10.0f)
{}

Object::Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const Vector3& vDirection, float fValocity) :
	m_vPos(pos),
	m_fSize(size),
	m_vColor(color),
	m_pRenderer(rend),
	m_vDirection(vDirection),
	m_fValocity(fValocity)
{}

Object::Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend, const Vector3& vDirection, float fValocity) :
	m_vPos(Vector3(x, y, z)),
	m_fSize(size),
	m_vColor(Vector4(r, g, b, a)),
	m_pRenderer(rend),
	m_vDirection(vDirection),
	m_fValocity(fValocity)
{}



Object::~Object()
{
}

void Object::Render()
{
	m_pRenderer->DrawSolidRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW());
}

// 

void Object::Move(float Elpsedtime = 0.0f)
{
	m_vPos += m_vDirection.Normalize() * m_fValocity * Elpsedtime;
}

void Object::Move(float x, float y, float z)
{
	m_vPos += Vector3(x, y, z);
}

void Object::Move(const Vector3& movepos, float fElpasedtime = 0.0f)
{
	m_vPos = m_vPos.Lerp(movepos, fElpasedtime);
}

//

void Object::Update(float fElpsedtime = 0.0f)
{
	Move(fElpsedtime);
}



/*------------------------------------------------------------------------------*/
void ObjectManagement::Update(float fElpsedtime)
{
	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
		(*p).Update(fElpsedtime);
}

void ObjectManagement::Render()
{
	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
		(*p).Render();
}