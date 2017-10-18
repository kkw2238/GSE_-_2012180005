#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Collision.h"
#include <vector>
#include <memory>

const int MAX_OBJECTS_COUNT = 5000;

class Object
{
private:
	Vector3		m_vPos;
	Vector3		m_vDirection;
	Vector4		m_vColor;

	float		m_fSize;
	float		m_fValocity;

	Renderer*	m_pRenderer;
	Collision	m_colAABB;

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

// 오브젝트 포인터를 넣어주고 비어 있는 공간에 새로운 오브젝트를 삽입.
class SceneManager
{
private:
	int m_iCurrentObjectCount;
	std::vector<std::unique_ptr<Object>> m_voObjects;

public:
	bool IsFull() const { return m_iCurrentObjectCount >= MAX_OBJECTS_COUNT; }

public:
	SceneManager() : m_iCurrentObjectCount(0) {};

	void Update(float fElpsedtime);
	void Render();

	int Add(Object& pObject);
	int Add(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const  Vector3& vDirection, float fValocity);

	void Destroy() { m_voObjects.clear(); }
	int RandomCreateObject(const int n, Renderer* rend);

};

