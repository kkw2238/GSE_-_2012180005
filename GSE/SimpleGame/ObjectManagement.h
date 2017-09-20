#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"

class ObjectManagement
{
private:
	Vector3		m_vPos;
	Vector4		m_vColor;

	float		m_fSize;
	
	Renderer*	m_pRenderer;

public:
	ObjectManagement();

	ObjectManagement(Vector3& pos, float size, Vector4& color, Renderer* rend);
	ObjectManagement(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend);

	~ObjectManagement();

public:
	virtual void Render();

public:
	void Move(Vector3& movepos);
	void Move(float x, float y, float z);
	void Move(Vector3& movepos, float time);


};

