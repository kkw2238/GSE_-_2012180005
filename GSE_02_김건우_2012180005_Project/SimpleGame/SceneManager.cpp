#include "stdafx.h"
#include "SceneManager.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Utily.h"

Object::Object()
{
}

Object::Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const Vector3& vDirection, float fValocity, ObjectType type, ObjectType team, float flife, RenderingLEVEL flevel) :
	m_vPos(pos),
	m_fSize(size),
	m_vColor(color),
	m_pRenderer(rend),
	m_fMaxLife(flife),
	m_vDirection(vDirection),
	m_fValocity(fValocity),
	m_colAABB(pos, size),
	m_fLife(flife),
	m_eObjectType(type),
	m_bLive(true),
	m_fActionTime(0.0f),
	m_fActionDelay(0.0f),
	m_eTeamType(team),
	m_fRenderingLevel(flevel)
{
	switch (type) {
	case OBJECT_BUILDING :
		m_fActionDelay = 1.0f;
		if (team == TEAM_1) m_itexID = m_pRenderer->CreatePngTexture("./Textures/PNGs/Hormor.png");
		if (team == TEAM_2) m_itexID = m_pRenderer->CreatePngTexture("./Textures/PNGs/Bart.png");
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

Object::~Object()
{
	for (auto iter = m_lpChildObject.begin(); iter != m_lpChildObject.end(); iter++)
		(*iter).reset();
}

void Object::Render()
{
	Vector4 fColor;

	if (m_eObjectType == OBJECT_BUILDING)
		m_pRenderer->DrawTexturedRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW(), m_itexID, m_fRenderingLevel);
	else
		m_pRenderer->DrawSolidRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW(), m_fRenderingLevel);

	if (m_eTeamType == TEAM_1)
		fColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	else fColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	if (m_eObjectType == OBJECT_BUILDING || m_eObjectType == OBJECT_CHARACTER)
		m_pRenderer->DrawSolidRectGauge(m_vPos.GetX(), m_vPos.GetY() + m_fSize, m_vPos.GetZ(), 
			m_fSize, 5.0f, fColor.GetX(), fColor.GetY(), fColor.GetZ(), fColor.GetW(), m_fLife / m_fMaxLife, m_fRenderingLevel);
}

// 

void Object::Move(float Elpsedtime = 0.0f)
{

	if (m_vPos.GetX() < -WIN_WIDTH / 2 || m_vPos.GetX() > WIN_WIDTH / 2) {
		if (m_eObjectType == OBJECT_BULLET || m_eObjectType == OBJECT_ARROW) {
			SetLive(false);
			return;
		}
		m_vDirection.SetX(m_vDirection.GetX() * -1);
	}

	if (m_vPos.GetY() < -WIN_HEIGHT / 2 || m_vPos.GetY() > WIN_HEIGHT / 2) {
		if (m_eObjectType == OBJECT_BULLET || m_eObjectType == OBJECT_ARROW) {
			SetLive(false);
			return;
		}
		m_vDirection.SetY(m_vDirection.GetY() * -1);
	}

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
	if (other == nullptr || other->m_eTeamType == m_eTeamType) return !m_bLive;

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
	m_fTeam1CharacterCooldown(0.0f),
	m_fTeam2CharacterCooldown(0.0f)
{
	InitSceneManager();
};

void SceneManager::Update(float fElpsedtime)
{
	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> ui(-WIN_WIDTH / 2, WIN_WIDTH / 2);
	std::uniform_int_distribution<int> uih(0, WIN_HEIGHT / 2);

	m_fTeam1CharacterCooldown += fElpsedtime;
	m_fTeam2CharacterCooldown += fElpsedtime;

	AddCharacter(Vector3(ui(engine), uih(engine), 0.0f), TEAM_1);

	for(auto iter = m_lBuildingObjects.begin(); iter != m_lBuildingObjects.end(); iter++) {
		std::shared_ptr<Object> pbuilding = *iter;
		if (pbuilding->CoolOn()) pbuilding->AddChildObject(*CreateNewObject(Vector3(pbuilding->GetPosition()), OBJECT_BULLET, pbuilding->GetTeam()));
	}
	
	if (m_lCharacterObjects.size() > 0) 
		for (auto iter = m_lCharacterObjects.begin(); iter != m_lCharacterObjects.end(); ++iter)
			if ((*iter)->CoolOn()) 	(*iter)->AddChildObject(*CreateNewObject(Vector3((*iter)->GetPosition()), OBJECT_ARROW, (*iter)->GetTeam()));
		

	for (auto characiter = m_lCharacterObjects.begin(); characiter != m_lCharacterObjects.end(); ++characiter) (*characiter)->Update(fElpsedtime);
	for (auto buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); ++buildingiter) (*buildingiter)->Update(fElpsedtime);
	for (auto arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); ++arrowiter) (*arrowiter)->Update(fElpsedtime);
	for (auto bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end(); ++bulletiter) (*bulletiter)->Update(fElpsedtime);

	CheckObjectCollision();
}

void SceneManager::CheckObjectCollision() 
{
	// 플레이어 -> 건물, 플레이어 -> 총알
	for (auto characteriter = m_lCharacterObjects.begin(); characteriter != m_lCharacterObjects.end(); characteriter++) {
		for(auto buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); buildingiter++) {
			if ((*characteriter) != nullptr && (*characteriter)->CollisionObject(*buildingiter)) {
				(*characteriter).reset();
				ClearDeletedObject(m_lArraowObjects, OBJECT_ARROW);
				break;
			}
		}

		for (auto iter = m_lBulletObjects.begin(); iter != m_lBulletObjects.end();) {
			if ((*iter) == nullptr) {
				iter++;
				continue;
			}

			if ((*characteriter) != nullptr && (*characteriter)->CollisionObject(*iter)) {
				(*characteriter).reset();
				ClearDeletedObject(m_lArraowObjects, OBJECT_ARROW);
				break;
			}

			else iter++;
		}

		for (auto arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); arrowiter++)
			if ((*arrowiter) != nullptr && (*arrowiter)->CollisionObject(*characteriter))
				(*arrowiter).reset();
	}
		
	// 건물 -> 화살, 총알 
	for (auto buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); buildingiter++) {
		if (*buildingiter == nullptr) continue;

		for (auto bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end(); bulletiter++)
			if ((*bulletiter) != nullptr && (*bulletiter)->CollisionObject(*buildingiter))
				(*bulletiter).reset();

		for (auto arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); arrowiter++)
			if ((*arrowiter) != nullptr && (*arrowiter)->CollisionObject(*buildingiter))
				(*arrowiter).reset();
	}

	ClearDeletedObject(m_lArraowObjects, OBJECT_ARROW);
	ClearDeletedObject(m_lBuildingObjects, OBJECT_BUILDING);
	ClearDeletedObject(m_lCharacterObjects, OBJECT_CHARACTER);
	ClearDeletedObject(m_lBulletObjects, OBJECT_BULLET);
}

void SceneManager::Render()
{
	for (auto characiter = m_lCharacterObjects.begin(); characiter != m_lCharacterObjects.end(); ++characiter)		(*characiter)->Render();
	for (auto buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); ++buildingiter)	(*buildingiter)->Render();
	for (auto arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); ++arrowiter)				(*arrowiter)->Render();
	for (auto bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end(); ++bulletiter)			(*bulletiter)->Render();

}

int SceneManager::AddCharacter(Vector3& vec3Pos, ObjectType type)
{ 
	if (type == TEAM_1 && m_fTeam1CharacterCooldown < 1.0f) return -1;
	if (type == TEAM_2 && m_fTeam2CharacterCooldown < 2.0f) return -1;

	CreateNewObject(vec3Pos, OBJECT_CHARACTER, type);
	if (type == TEAM_1) m_fTeam1CharacterCooldown = 0.0f;
	if (type == TEAM_2) m_fTeam2CharacterCooldown = 0.0f;
		
	return 1;
}

std::shared_ptr<Object>* SceneManager::CreateNewObject(Vector3& pos, ObjectType type, ObjectType team)
{
	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> ui(-250, 250);

	Vector3 vecDirection;
	Vector4 vecColor;
	float fsize = 0.0f;
	float fValocity = 0.0f;
	float fLife = 0.0f;
	float fLevel = LEVEL_GOD;

	switch (type) {
	case OBJECT_BUILDING:
		vecColor	 = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		fValocity	 = 0.0f;
		fsize		 = 100.0f;
		fLife		 = 500.0f;
		vecDirection = Vector3(0.0f, 0.0f, 0.0f);
		break;
	case OBJECT_CHARACTER:
		fValocity	 = 300.0f;
		fsize		 = 30.0f;
		fLife		 = 100.0f;
		vecColor = team == TEAM_1 ? Vector4(1.0f, 0.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		break;
	case OBJECT_BULLET:
		fValocity	 = 300.0f;
		fsize		 = 4.0f;
		fLife		 = 15.0f;
		vecColor	 = team == TEAM_1 ? Vector4(1.0f, 0.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		break;
	case OBJECT_ARROW:
		fValocity	 = 100.0f;
		fsize		 = 4.0f;
		fLife		 = 15.0f;
		vecColor	 = team == TEAM_1 ? Vector4(0.5f, 0.2f, 0.7f, 1.0f) : Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		break;
	}

	if (!IsFull()) {
		std::shared_ptr<Object> pNewObject(new Object(pos, fsize, vecColor, m_pRenderer, vecDirection, fValocity, type, team, fLife , fLevel));
		switch (type)
		{
		case OBJECT_BUILDING:
			m_lBuildingObjects.push_back(std::shared_ptr<Object>(pNewObject));
			break;
		case OBJECT_ARROW:
			m_lArraowObjects.push_back(std::shared_ptr<Object>(pNewObject));
			break;
		case OBJECT_BULLET:
			m_lBulletObjects.push_back(std::shared_ptr<Object>(pNewObject));
			break;
		case OBJECT_CHARACTER:
			m_lCharacterObjects.push_back(std::shared_ptr<Object>(pNewObject));
			break;
		}
		return &pNewObject;
	}

	return nullptr;
}

int SceneManager::RandomCreateObject(const int n)
{
//	std::mt19937 engine((unsigned int)time(NULL));
//
//	std::uniform_int_distribution<int> ui(-250, 250);
//	std::uniform_real_distribution<float> uf(0.0f, 1.0f);
//	std::uniform_real_distribution<float> ufspd(0.0f, 100.0f);
//
//	int i = 0;
//	for (i = 0; i < n; i++)
//	{
//		if (!IsFull())
//		{
//			Vector3 vecPos = Vector3(ui(engine) - 100, 100 - ui(engine), 0.0f);
//			Vector3 vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
//			Vector4 vecColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//
//			Object tmp(vecPos, 20.0f, vecColor, m_pRenderer, vecDirection, 10.0f, OBJECT_CHARACTER, 100.0f);
//			Add(tmp);
//		}
//		else
//			return i;
//	}
//
	return 1;
}

void SceneManager::InitSceneManager()
{
	m_pRenderer = new Renderer(WIN_WIDTH, WIN_HEIGHT);
	m_sScreen = Screen(WIN_WIDTH, WIN_HEIGHT);
	m_tTime = Time();
	CreateNewObject(Vector3(+000.0f, -300.0f, 0.0f), OBJECT_BUILDING, TEAM_2);
	CreateNewObject(Vector3(+150.0f, -150.0f, 0.0f), OBJECT_BUILDING, TEAM_2);
	CreateNewObject(Vector3(-150.0f, -150.0f, 0.0f), OBJECT_BUILDING, TEAM_2);
	CreateNewObject(Vector3(+000.0f, +300.0f, 0.0f), OBJECT_BUILDING, TEAM_1);
	CreateNewObject(Vector3(+150.0f, +150.0f, 0.0f), OBJECT_BUILDING, TEAM_1);
	CreateNewObject(Vector3(-150.0f, +150.0f, 0.0f), OBJECT_BUILDING, TEAM_1);

}



void SceneManager::Destroy() 
{ 
	m_lBuildingObjects.clear();

	m_lArraowObjects.clear();

	m_lBulletObjects.clear();

	m_lCharacterObjects.clear();

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