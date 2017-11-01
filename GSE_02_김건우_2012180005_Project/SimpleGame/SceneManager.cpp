#include "stdafx.h"
#include "SceneManager.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Utily.h"

Object::Object()
{
}

Object::Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const Vector3& vDirection, float fValocity, ObjectType type, float flife) :
	m_vPos(pos),
	m_fSize(size),
	m_vColor(color),
	m_pRenderer(rend),
	m_vDirection(vDirection),
	m_fValocity(fValocity),
	m_colAABB(pos, size),
	m_fLife(flife),
	m_eObjectType(type),
	m_bLive(true)
{
}

Object::Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend, const Vector3& vDirection, float fValocity, ObjectType type, float flife) :
	m_vPos(Vector3(x, y, z)),
	m_fSize(size),
	m_vColor(Vector4(r, g, b, a)),
	m_pRenderer(rend),
	m_vDirection(vDirection),
	m_fValocity(fValocity),
	m_colAABB(Vector3(x, y, z), size),
	m_fLife(flife),
	m_eObjectType(type),
	m_bLive(true)
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
	if (m_eObjectType == OBJECT_BUILDING) return;
	Vector3 a = m_vDirection.Normalize() * m_fValocity * Elpsedtime;
	m_vPos += m_vDirection.Normalize() * m_fValocity * Elpsedtime;

}

//

void Object::Update(float fElpsedtime = 0.0f)
{
	Move(fElpsedtime);
	m_colAABB.RefreshCollision(Vector2(m_vPos.x, m_vPos.y), m_fSize);
}

//

Object* Object::CollisionObject(Object& other)
{
	if (m_eObjectType == other.GetType()) return nullptr;
	if (m_eObjectType == OBJECT_BUILDING && other.GetType() == OBJECT_BULLET) return nullptr;

	if (m_colAABB.IsCollision(other.m_colAABB)) {

		Damage(other.GetLife());
		if (other.GetType() == OBJECT_BULLET) other.SetDead(false);
		else other.Damage(GetLife());

		return &other;
	}
	return nullptr;
}

/*------------------------------------------------------------------------------*/
void SceneManager::Update(float fElpsedtime)
{
	m_fDelayTime += fElpsedtime;

	if (m_fDelayTime > 0.5f) {
		if (m_voObjects.size() == 0);
		else if (m_voObjects[0]->GetType() == OBJECT_BUILDING) {
			Add(Vector3(0.0f, 0.0f, 0.0f), OBJECT_BULLET);
			m_fDelayTime = 0.0f;
		}
	};
	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
		(*(*p)).Update(fElpsedtime);

	CheckObjectCollision();
}

void SceneManager::CheckObjectCollision() 
{
	for (int i = 0; i < m_voObjects.size(); i++)
	{
		for (int j = i + 1; j < m_voObjects.size(); j++) {
			Object* oCollObject = m_voObjects[i]->CollisionObject(*(m_voObjects[j]));
			if (oCollObject != nullptr) {
				if (!m_voObjects[i]->isLive()) m_voObjects.erase(m_voObjects.begin() + i);
					
			}
		}
		if (m_sScreen.CollisionOther(m_voObjects[i]->GetCollision()) != -1)
			m_voObjects[i]->SetDirection(m_voObjects[i]->GetDirection() * -1);
	}
}

void SceneManager::Render()
{
	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
		(*(*p)).Render();
}

int SceneManager::Add(Object& pObject) 
{ 
	if (!IsFull()) {
		m_voObjects.push_back(std::unique_ptr<Object>(new Object(pObject)));
		return m_iCurrentObjectCount++;
	}
	return -1;
}

int SceneManager::Add(Vector3& pos, ObjectType type)
{

	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> ui(-250, 250);

	Vector3 vecDirection;
	Vector4 vecColor;
	float fsize = 0.0f;
	float fValocity = 0.0f;
	float fLife = 0.0f;

	switch (type) {
	case OBJECT_BUILDING:
		vecColor	 = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		fValocity	 = 0.0f;
		fsize		 = 50.0f;
		fLife		 = 500.0f;
		vecDirection = Vector3(0.0f, 0.0f, 0.0f);
		break;
	case OBJECT_CHARACTER:
		fValocity	 = 100.0f;
		fsize		 = 10.0f;
		fLife		 = 10.0f;
		vecColor	 = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		break;
	case OBJECT_BULLET:
		fValocity	 = 300.0f;
		fsize		 = 2.0f;
		fLife		 = 20.0f;
		vecColor	 = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		break;
	case OBJECT_ARROW:
		fValocity	 = 100.0f;
		fsize		 = 2.0f;
		vecColor	 = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vecDirection = Vector3(1.0f, 1.0f, 0.0f);
		break;
	}

	if (!IsFull()) {
		m_voObjects.push_back(std::unique_ptr<Object>(new Object(pos, fsize, vecColor, m_pRenderer, vecDirection, fValocity, type, fLife)));
		return m_iCurrentObjectCount++;
	}
	return -1;
}

int SceneManager::RandomCreateObject(const int n)
{
	std::mt19937 engine((unsigned int)time(NULL));

	std::uniform_int_distribution<int> ui(-250, 250);
	std::uniform_real_distribution<float> uf(0.0f, 1.0f);
	std::uniform_real_distribution<float> ufspd(0.0f, 100.0f);

	int i = 0;
	for (i = 0; i < n; i++)
	{
		if (!IsFull())
		{
			Vector3 vecPos = Vector3(ui(engine) - 100, 100 - ui(engine), 0.0f);
			Vector3 vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
			//Vector4 vecColor = Vector4(uf(engine), uf(engine), uf(engine), uf(engine));
			Vector4 vecColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			Object tmp(vecPos, 20.0f, vecColor, m_pRenderer, vecDirection, 10.0f, OBJECT_CHARACTER, 100.0f);
			Add(tmp);
		}
		else
			return i;
	}

	return i;
}

void SceneManager::InitSceneManager()
{
	m_pRenderer = new Renderer(500, 500);
	m_sScreen = Screen(500, 500);
	m_tTime = Time();
	Add(Vector3(0.0f, 0.0f, 0.0f), OBJECT_BUILDING);
}

void SceneManager::Destroy() 
{ 
	m_voObjects.clear(); 

	delete(m_pRenderer);
}

void SceneManager::Run()
{
	m_tTime.Update(60.0f);
	float fElapsedTime = m_tTime.Tick();

	Update(fElapsedTime);
	Render();
	
	std::string titlename = m_tTime.GetFrameTime();
	glutSetWindowTitle(titlename.c_str());
	m_tTime.Tock();
}