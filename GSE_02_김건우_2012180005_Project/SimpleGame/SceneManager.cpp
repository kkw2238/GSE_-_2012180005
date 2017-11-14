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
	m_bLive(true),
	m_fActionTime(0.0f),
	m_fActionDelay(0.0f)
{
	switch (type) {
	case OBJECT_BUILDING :
		m_fActionDelay = 0.5f;
		m_itexID = m_pRenderer->CreatePngTexture("./Textures/PNGs/Hormor.png");
		break;
	case OBJECT_CHARACTER : 
		m_fActionDelay = 0.5f;
		break;
	case OBJECT_BULLET : 
		break;
	case OBJECT_ARROW :
		break;
	}
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
	m_bLive(true),
	m_fActionTime(0.0f),
	m_fActionDelay(0.0f)
{
	switch (type) {
	case OBJECT_BUILDING:
		m_fActionDelay = 0.5f;
		m_itexID = m_pRenderer->CreatePngTexture("./Textures/PNGs/Hormor.png");
		std::cout << m_itexID << std::endl;
		break;
	case OBJECT_CHARACTER:
		m_fActionDelay = 0.5f;
		break;
	case OBJECT_BULLET:
		break;
	case OBJECT_ARROW:
		break;
	}


}

Object::~Object()
{
	for (auto iter = m_lpChildObject.begin(); iter != m_lpChildObject.end(); iter++)
		(*iter).reset();
}

void Object::Render()
{
	if (m_eObjectType == OBJECT_BUILDING)
		m_pRenderer->DrawTexturedRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW(), m_itexID);
	else
		m_pRenderer->DrawSolidRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW());
}

// 

void Object::Move(float Elpsedtime = 0.0f)
{

	Vector3 a = m_vDirection.Normalize() * m_fValocity * Elpsedtime;
	m_vPos += m_vDirection.Normalize() * m_fValocity * Elpsedtime;

}

//

void Object::Update(float fElpsedtime = 0.0f)
{
	m_fActionTime += fElpsedtime;
	ClearDeletedObject(m_lpChildObject, OBJECT_ARROW);

	if(m_eObjectType != OBJECT_BUILDING)
		Move(fElpsedtime);

	m_colAABB.RefreshCollision(Vector2(m_vPos.x, m_vPos.y), m_fSize);
}

//

void Object::Damage(const float fDamage)
{
	m_fLife -= fDamage;
	if (m_eObjectType == OBJECT_BULLET) SetLive(false);
	if (m_eObjectType == OBJECT_ARROW)  SetLive(false);
	if (m_fLife <= 0.0f) SetLive(false);
}

bool Object::CollisionObject(std::shared_ptr<Object>& other)
{
	if (other == nullptr) return !m_bLive;

	if (m_colAABB.IsCollision(other->m_colAABB)) {

		float damage = GetLife();
		Damage(other->GetLife());
		other->Damage(damage);

		if (!other->isLive()) other.reset();
		return !m_bLive;
	}
	return !m_bLive;
}

/*------------------------------------------------------------------------------*/
SceneManager::SceneManager() : 
	m_iCurrentObjectCount(0), 
	m_fDelayTime(0.0f) 
{
	m_voObjects = VISP(4);
	InitSceneManager();
};

void SceneManager::Update(float fElpsedtime)
{
	m_fDelayTime += fElpsedtime;

	if (m_voObjects[(int)OBJECT_BUILDING].size() > 0) {
		std::shared_ptr<Object> pbuilding = *m_voObjects[(int)OBJECT_BUILDING].begin();
		if (pbuilding->CoolOn()) pbuilding->AddChildObject(*CreateNewObject(Vector3(0.0f, 0.0f, 0.0f), OBJECT_BULLET));
	}
	
	if (m_voObjects[(int)OBJECT_CHARACTER].size() > 0) 
		for (auto iter = m_voObjects[(int)OBJECT_CHARACTER].begin(); iter != m_voObjects[(int)OBJECT_CHARACTER].end(); ++iter) 
			if ((*iter)->CoolOn()) 	(*iter)->AddChildObject(*CreateNewObject(Vector3((*iter)->GetPosition()), OBJECT_ARROW));
		
	
	for (auto vectoriter = m_voObjects.begin(); vectoriter != m_voObjects.end(); vectoriter++) 
		for (auto listiter = (*vectoriter).begin(); listiter != (*vectoriter).end(); listiter++) 	
			(**listiter).Update(fElpsedtime);

	CheckObjectCollision();
}

void SceneManager::CheckObjectCollision() 
{
	// 플레이어 -> 건물, 플레이어 -> 총알
	for (auto characteriter = m_voObjects[OBJECT_CHARACTER].begin(); characteriter != m_voObjects[OBJECT_CHARACTER].end(); characteriter++) {
		
		if (m_voObjects[OBJECT_BUILDING].begin() != m_voObjects[OBJECT_BUILDING].end() && *m_voObjects[OBJECT_BUILDING].begin() != nullptr) {

			if ((*characteriter)->CollisionObject(*m_voObjects[OBJECT_BUILDING].begin())) {
				(*characteriter).reset();
				ClearDeletedObject(m_voObjects[OBJECT_ARROW], OBJECT_ARROW);
				continue;
			};
		}

		for (auto iter = m_voObjects[OBJECT_BULLET].begin(); iter != m_voObjects[OBJECT_BULLET].end();) {
			if ((*iter) == nullptr) {
				iter++;
				continue;
			}

			if ((*characteriter)->CollisionObject(*iter)) {
				(*characteriter).reset();
				ClearDeletedObject(m_voObjects[OBJECT_ARROW], OBJECT_ARROW);
				break;
			}

			else iter++;
		}
	}
		
	// 화살 -> 건물 
	for (auto arrowiter = m_voObjects[OBJECT_ARROW].begin(); arrowiter != m_voObjects[OBJECT_ARROW].end(); arrowiter++) {

		if (m_voObjects[OBJECT_BUILDING].size() > 0 && (*arrowiter)->CollisionObject(*m_voObjects[OBJECT_BUILDING].begin())) {
			(*arrowiter).reset();
			continue;
		};

	}

	ClearDeletedObject(m_voObjects[OBJECT_ARROW], OBJECT_ARROW);
	ClearDeletedObject(m_voObjects[OBJECT_BUILDING], OBJECT_BUILDING);
	ClearDeletedObject(m_voObjects[OBJECT_CHARACTER], OBJECT_CHARACTER);
	ClearDeletedObject(m_voObjects[OBJECT_BULLET], OBJECT_BULLET);
}

void SceneManager::Render()
{
	for (auto vectoriter = m_voObjects.begin(); vectoriter != m_voObjects.end(); vectoriter++)
		for (auto listiter = (*vectoriter).begin(); listiter != (*vectoriter).end(); listiter++)
			(**listiter).Render();
}

int SceneManager::Add(Object& pObject) 
{ 
	if (!IsFull()) {
		m_voObjects[(int)pObject.GetType()].push_back(std::shared_ptr<Object>(new Object(pObject)));
		return m_iCurrentObjectCount++;
	}
	return -1;
}

std::shared_ptr<Object>* SceneManager::CreateNewObject(Vector3& pos, ObjectType type)
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
		fValocity	 = 300.0f;
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
		fLife		 = 10.0f;
		vecColor	 = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		break;
	}

	if (!IsFull()) {
		std::shared_ptr<Object> pNewObject(new Object(pos, fsize, vecColor, m_pRenderer, vecDirection, fValocity, type, fLife));
		m_voObjects[(int)type].emplace_back(pNewObject);
		return &pNewObject;
	}

	return nullptr;
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
	CreateNewObject(Vector3(0.0f, 0.0f, 0.0f), OBJECT_BUILDING);
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