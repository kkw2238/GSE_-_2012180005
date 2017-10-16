#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include <vector>

class Object
{
private:
	Vector3		m_vPos;
	Vector3		m_vDirection;
	Vector4		m_vColor;

	float		m_fSize;
	float		m_fValocity;

	Renderer*	m_pRenderer;

public:
	Object();

	Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend);
	Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend);

	Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const  Vector3& vDirection, float fValocity);
	Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend, const Vector3& vDirection, float fValocity);

	~Object();

public:
	virtual void Render();
	virtual void Update(float fElpsedtime);

public:
	void Move(float Elpsedtime);
	void Move(float x, float y, float z);
	void Move(const Vector3& movepos, float fElpsedtime);

	void SetValocity(const float fValocity) { m_fValocity = fValocity; }
	void SetDirection(const Vector3& vDirection) { m_vDirection = vDirection; }
};

class ObjectManagement
{
private:
	std::vector<Object> m_voObjects;

public:
	ObjectManagement() {};

	void Update(float fElpsedtime);
	void Render();
	void Add(Object& pObject) { m_voObjects.push_back(pObject); }
};

