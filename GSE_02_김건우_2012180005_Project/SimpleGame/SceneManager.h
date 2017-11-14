#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Collision.h"
#include "Screen.h"
#include "Time.h"

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
	float		m_fLife;

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
	void SetColor(Vector4& vColor) { m_vColor = vColor; }
	
	void Damage(const float fDamage) { 
		m_fLife -= fDamage; 
	}

	bool isDead() const { return m_fLife < 0.0f; }

	Vector3 GetDirection() const { return m_vDirection; }
	Collision GetCollision() const { return m_colAABB; }

	Object* CollisionObject(Object& other);
};

// ������Ʈ �����͸� �־��ְ� ��� �ִ� ������ ���ο� ������Ʈ�� ����.
class SceneManager
{
private:
	int m_iCurrentObjectCount;

	Renderer* m_pRenderer;
	Screen m_sScreen;
	Time m_tTime;

	std::vector<std::unique_ptr<Object>> m_voObjects;

public:
	bool IsFull() const { return m_iCurrentObjectCount >= MAX_OBJECTS_COUNT; }

public:
	SceneManager() : m_iCurrentObjectCount(0) { InitSceneManager(); };
	~SceneManager() { Destroy(); }

	void Run();
	void Update(float fElpsedtime);
	void Render();
	void ObjectDamage(float fElapsedTime);
	void CheckObjectCollision();
	void InitSceneManager();
	
	int Add(Object& pObject);
	int Add(const Vector3& pos, float size, const Vector4& color, const  Vector3& vDirection, float fValocity);

	void Destroy();

	int RandomCreateObject(const int n);

};

