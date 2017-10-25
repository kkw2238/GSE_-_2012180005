#include "stdafx.h"
#include "SceneManager.h"
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
	m_fValocity(10.0f),
	m_colAABB(pos, size),
	m_fLife(5.0f)
{}

Object::Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend) :
	m_vPos(Vector3(x, y, z)),
	m_fSize(size),
	m_vColor(Vector4(r, g, b, a)),
	m_pRenderer(rend),
	m_vDirection(Vector3()),
	m_fValocity(10.0f),
	m_colAABB(Vector3(x, y, z), size),
	m_fLife(5.0f)
{}

Object::Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const Vector3& vDirection, float fValocity) :
	m_vPos(pos),
	m_fSize(size),
	m_vColor(color),
	m_pRenderer(rend),
	m_vDirection(vDirection),
	m_fValocity(fValocity),
	m_colAABB(pos, size),
	m_fLife(5.0f)
{}

Object::Object(float x, float y, float z, float size, float r, float g, float b, float a, Renderer* rend, const Vector3& vDirection, float fValocity) :
	m_vPos(Vector3(x, y, z)),
	m_fSize(size),
	m_vColor(Vector4(r, g, b, a)),
	m_pRenderer(rend),
	m_vDirection(vDirection),
	m_fValocity(fValocity),
	m_colAABB(Vector3(x, y, z), size),
	m_fLife(5.0f)
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
	m_colAABB.RefreshCollision(Vector2(m_vPos.x, m_vPos.y), m_fSize);
}

//

Object* Object::CollisionObject(Object& other)
{
	if (m_colAABB.IsCollision(other.m_colAABB))
		return &other;

	return nullptr;
}

/*------------------------------------------------------------------------------*/
void SceneManager::Update(float fElpsedtime)
{
	ObjectDamage(fElpsedtime);

	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
		(*(*p)).Update(fElpsedtime);

	CheckObjectCollision();
}

void SceneManager::CheckObjectCollision() 
{
	Vector4 Red(1.0f, 0.0f, 0.0f, 1.0f);
	Vector4 White(1.0f, 1.0f, 1.0f, 1.0f);

	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
		(*p)->SetColor(White);

	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
	{
		for (auto iter = p + 1; iter != m_voObjects.end(); iter++) {
			Object* oCollObject = (*(*p)).CollisionObject(*(*iter));
			if (oCollObject != nullptr) {
				(*p)->SetColor(Red);
				oCollObject->SetColor(Red);
			}
		}

		if (m_sScreen.CollisionOther((*p)->GetCollision()) != -1)
			(*p)->SetDirection((*p)->GetDirection() * -1);
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

int SceneManager::Add(const Vector3& pos, float size, const Vector4& color, const  Vector3& vDirection, float fValocity)
{
	if (!IsFull()) {
		m_voObjects.push_back(std::unique_ptr<Object>(new Object(pos, size, color, m_pRenderer, vDirection, fValocity)));
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

			Object tmp(vecPos, 20.0f, vecColor, m_pRenderer, vecDirection, ufspd(engine));
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
	
	m_tTime.Tock();
}

void SceneManager::ObjectDamage(float fElapsedTime)
{
	for (auto p = m_voObjects.begin(); p != m_voObjects.end(); p++)
	{
		(*(*p)).Damage(fElapsedTime);
		if ((*(*p)).isDead())
		{
			m_voObjects.erase(p);
		}
		
	}
	std::cout << m_voObjects.size() << std::endl;
}