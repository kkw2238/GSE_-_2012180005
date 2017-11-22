#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Collision.h"
#include "Screen.h"
#include "Time.h"


const int MAX_OBJECTS_COUNT = 5000;

const float LEVEL_GOD			= 0.0f;
const float LEVEL_SKY			= 0.1f;
const float LEVEL_GROUND		= 0.2f;
const float LEVEL_UNDERGROUND	= 0.3f;

enum ObjectType { OBJECT_BUILDING, OBJECT_CHARACTER, OBJECT_BULLET, OBJECT_ARROW, TEAM_1, TEAM_2 };

class Object
{
	using RenderingLEVEL = float;
private:
	Vector3										m_vPos;
	Vector3										m_vDirection;
	Vector4										m_vColor;

	bool										m_bLive;

	float										m_fSize;
	float										m_fValocity;
	float										m_fLife;
	float										m_fMaxLife;
	float										m_fActionTime;
	float										m_fActionDelay;

	int											m_itexID;
	Renderer*									m_pRenderer;
	Collision									m_colAABB;
	ObjectType									m_eObjectType;
	ObjectType									m_eTeamType;
	RenderingLEVEL								m_fRenderingLevel;

	std::list<std::shared_ptr<Object>>			m_lpChildObject;
	
public:
	Object();

	Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const  Vector3& vDirection, float fValocity, ObjectType type, ObjectType team, float Life, RenderingLEVEL flevel);
	
	~Object();

public:
	virtual void Render();
	virtual void Update(float fElpsedtime);

public:
	void Move(float Elpsedtime);

	void SetValocity(const float fValocity) { m_fValocity = fValocity; }
	void SetDirection(const Vector3& vDirection) { m_vDirection = vDirection; }
	void SetColor(Vector4& vColor) { m_vColor = vColor; }
	void SetLive(bool isDead) { m_bLive = isDead; }

	void AddChildObject(std::shared_ptr<Object>& child) {
		m_lpChildObject.push_back(child);
		m_fActionTime = 0.0f;
	}

	void Damage(const float fDamage);

	bool isLive() const { return m_bLive; }
	bool CoolOn() const { return m_fActionTime > m_fActionDelay; }

	float GetLife() const { return m_fLife; }

	Vector3 GetDirection() const { return m_vDirection; }
	Vector3 GetPosition() const { return m_vPos; }
	Collision GetCollision() const { return m_colAABB; }
	ObjectType GetType() const { return m_eObjectType; }
	ObjectType GetTeam() const { return m_eTeamType; }

	bool CollisionObject(std::shared_ptr<Object>& other);
};

// 오브젝트 포인터를 넣어주고 비어 있는 공간에 새로운 오브젝트를 삽입.
class SceneManager
{
public:
	using VISP = std::vector<std::list<std::shared_ptr<Object>>>;

private:
	int m_iCurrentObjectCount;
	float m_fTeam1CharacterCooldown;
	float m_fTeam2CharacterCooldown;

	Renderer* m_pRenderer;
	Screen m_sScreen;
	Time m_tTime;

	std::list<std::shared_ptr<Object>>	m_lBuildingObjects;
	std::list<std::shared_ptr<Object>>	m_lBulletObjects;
	std::list<std::shared_ptr<Object>>	m_lCharacterObjects;
	std::list<std::shared_ptr<Object>>	m_lArraowObjects;
public:
	bool IsFull() const { return m_iCurrentObjectCount >= MAX_OBJECTS_COUNT; }

public:
	SceneManager();
	~SceneManager() { Destroy(); }

	void Run();
	void Update(float fElpsedtime);
	void Render();
	void CheckObjectCollision();
	void InitSceneManager();
	
	int AddCharacter(Vector3& vec3Pos, ObjectType type);
	std::shared_ptr<Object>* CreateNewObject(Vector3& pos, ObjectType type, ObjectType team);

	void Destroy();

	int RandomCreateObject(const int n);

};

