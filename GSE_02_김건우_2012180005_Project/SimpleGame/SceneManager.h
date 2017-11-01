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
static enum ObjectType { OBJECT_BUILDING, OBJECT_CHARACTER, OBJECT_BULLET, OBJECT_ARROW };

class Object
{
private:
	Vector3						m_vPos;
	Vector3						m_vDirection;
	Vector4						m_vColor;

	bool						m_bLive;

	float						m_fSize;
	float						m_fValocity;
	float						m_fLife;

	Renderer*					m_pRenderer;
	Collision					m_colAABB;
	ObjectType					m_eObjectType;
	
public:
	Object();

	Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const  Vector3& vDirection, float fValocity, ObjectType type, float Life);
	Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend, const Vector3& vDirection, float fValocity, ObjectType type, float Life);

	~Object();

public:
	virtual void Render();
	virtual void Update(float fElpsedtime);

public:
	void Move(float Elpsedtime);

	void SetValocity(const float fValocity) { m_fValocity = fValocity; }
	void SetDirection(const Vector3& vDirection) { m_vDirection = vDirection; }
	void SetColor(Vector4& vColor) { m_vColor = vColor; }
	void SetDead(bool isDead) { m_bLive = isDead; }

	void Damage(const float fDamage) { 
		m_fLife -= fDamage; 
		if (m_fLife <= 0.0f) SetDead(false);
	}

	bool isLive() const { return m_bLive; }

	float GetLife() const { return m_fLife; }

	Vector3 GetDirection() const { return m_vDirection; }
	Collision GetCollision() const { return m_colAABB; }
	ObjectType GetType() const { return m_eObjectType; }

	Object* CollisionObject(Object& other);
};

// 오브젝트 포인터를 넣어주고 비어 있는 공간에 새로운 오브젝트를 삽입.
class SceneManager
{

private:
	int m_iCurrentObjectCount;
	float m_fDelayTime;

	Renderer* m_pRenderer;
	Screen m_sScreen;
	Time m_tTime;

	std::vector<std::unique_ptr<Object>> m_voObjects;

public:
	bool IsFull() const { return m_iCurrentObjectCount >= MAX_OBJECTS_COUNT; }

public:
	SceneManager() : m_iCurrentObjectCount(0), m_fDelayTime(0.0f){ InitSceneManager(); };
	~SceneManager() { Destroy(); }

	void Run();
	void Update(float fElpsedtime);
	void Render();
	void CheckObjectCollision();
	void InitSceneManager();
	
	int Add(Object& pObject);
	int Add(Vector3& pos, ObjectType type);

	void Destroy();

	int RandomCreateObject(const int n);

};

