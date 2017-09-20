#include "stdafx.h"
#include "ObjectManagement.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"


ObjectManagement::ObjectManagement()
{
}

ObjectManagement::ObjectManagement(Vector3& pos, float size, Vector4& color, Renderer* rend) :
	m_vPos(pos),
	m_fSize(size),
	m_vColor(color),
	m_pRenderer(rend)
{

}
ObjectManagement::ObjectManagement(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend) :
	m_vPos(Vector3(x, y, z)),
	m_fSize(size),
	m_vColor(Vector4(r, g, b, a)),
	m_pRenderer(rend)
{

}

ObjectManagement::~ObjectManagement()
{
}

/*--------------------------------------



--------------------------------------*/

void ObjectManagement::Render()
{
	m_pRenderer->DrawSolidRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW());
}

void ObjectManagement::Move(Vector3& movepos)
{
	m_vPos += movepos;
}

void ObjectManagement::Move(float x, float y, float z)
{
	m_vPos += Vector3(x, y, z);
}
void ObjectManagement::Move(Vector3& movepos, float time)
{
	m_vPos += m_vPos.Lerp(movepos, time);

	int a = 0;
	a += 5;
}