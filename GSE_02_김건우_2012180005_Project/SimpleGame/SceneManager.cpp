#include "stdafx.h"
#include "SceneManager.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Utily.h"

Object::Object()
{
}

Object::Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const Vector3& vDirection, float fValocity, ObjectType type, TeamType team, float flife, int texID, RenderingLEVEL flevel) :
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
	m_fRenderingLevel(flevel),
	m_iMaxSpriteCount(1),
	m_iCurSpirteCount(0),
	m_itexID(texID)
{
	switch (type) {
	case OBJECT_BUILDING :
		m_fActionDelay = 1.0f;
		m_iMaxSpriteCount = 1;
		m_iSpriteXCount = 1;
		m_iSpriteYCount = 1;
		break;

	case OBJECT_CHARACTER : 
		m_fActionDelay = 0.5f;
		
		m_iMaxSpriteCount = 58;
		m_iSpriteXCount = 8;
		m_iSpriteYCount = 8;
		m_vDirection = m_vDirection.Normalize();
		break;

	case OBJECT_BULLET : 
		m_vDirection = m_vDirection.Normalize();
		break;

	case OBJECT_ARROW :
		m_vDirection = m_vDirection.Normalize();
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
	else if (m_eObjectType == OBJECT_CHARACTER)
		m_pRenderer->DrawTexturedRectSeq(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW(),
			m_itexID, m_iCurSpirteCount % m_iSpriteXCount, (m_iCurSpirteCount / m_iSpriteYCount) % m_iSpriteYCount, m_iSpriteXCount, m_iSpriteYCount, m_fRenderingLevel);
	else if (m_eObjectType == OBJECT_BULLET) {
			m_pRenderer->DrawParticle(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), 5.0f, 1.0f, 1.0f, 1.0f, 1.0f,
				GetDirection().x * -1, GetDirection().y * -1, m_itexID, m_fActionTime, LEVEL_BULLET);
	}
	else
		m_pRenderer->DrawSolidRect(m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ(), m_fSize, m_vColor.GetX(), m_vColor.GetY(), m_vColor.GetZ(), m_vColor.GetW(), m_fRenderingLevel);


	if (m_eTeamType == TEAM_1)	
		fColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	else if((m_eTeamType == TEAM_2))
		fColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	if (m_eObjectType == OBJECT_BUILDING || m_eObjectType == OBJECT_CHARACTER) {

		sprintf_s(m_pHPbuf, "HP : %d", (int)m_fLife);

		m_pRenderer->DrawSolidRectGauge(m_vPos.GetX(), m_vPos.GetY() + m_fSize / 2, m_vPos.GetZ(),
			m_fSize, 5.0f, fColor.GetX(), fColor.GetY(), fColor.GetZ(), fColor.GetW(), m_fLife / m_fMaxLife, m_fRenderingLevel);

		m_pRenderer->DrawTextW(m_vPos.GetX() - 25 , m_vPos.GetY() + m_fSize / 2, GLUT_BITMAP_8_BY_13, 1.0f, 1.0f, 1.0f, m_pHPbuf);
	}
	m_iCurSpirteCount = ++m_iCurSpirteCount >= m_iMaxSpriteCount ? 0 : m_iCurSpirteCount;
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

	m_vPos += m_vDirection * m_fValocity * Elpsedtime;

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
	m_fTeam2CharacterCooldown(0.0f),
	m_fSnowTime(0.0f)
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
	m_fSnowTime += fElpsedtime;

	AddCharacter(Vector3(ui(engine), uih(engine), 0.0f), TEAM_1);

	for(auto iter = m_lBuildingObjects.begin(); iter != m_lBuildingObjects.end(); iter++) {
		std::shared_ptr<Object> pbuilding = *iter;
		if (pbuilding->CoolOn()) pbuilding->AddChildObject(*CreateNewObject(Vector3(pbuilding->GetPosition()), OBJECT_BULLET, pbuilding->GetTeam()));
	}
	
	if (m_lCharacterObjects.size() > 0) 
		for (auto iter = m_lCharacterObjects.begin(); iter != m_lCharacterObjects.end(); ++iter)
			if ((*iter)->CoolOn()) 	
				(*iter)->AddChildObject(*CreateNewObject(Vector3((*iter)->GetPosition()), OBJECT_ARROW, (*iter)->GetTeam()));
		

	for (auto characiter = m_lCharacterObjects.begin(); characiter != m_lCharacterObjects.end(); ++characiter) (*characiter)->Update(fElpsedtime);
	for (auto buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); ++buildingiter) (*buildingiter)->Update(fElpsedtime);
	for (auto arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); ++arrowiter) (*arrowiter)->Update(fElpsedtime);
	for (auto bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end(); ++bulletiter) (*bulletiter)->Update(fElpsedtime);

	CheckObjectCollision();
}

void SceneManager::CheckObjectCollision() 
{

	std::list<std::shared_ptr<Object>>::iterator characteriter;
	std::list<std::shared_ptr<Object>>::iterator buildingiter;
	std::list<std::shared_ptr<Object>>::iterator arrowiter;
	std::list<std::shared_ptr<Object>>::iterator bulletiter;

	// 플레이어 -> 건물, 플레이어 -> 총알
	for (characteriter = m_lCharacterObjects.begin(); characteriter != m_lCharacterObjects.end(); characteriter++) {
		for(buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); buildingiter++) {
			if ((*characteriter) != nullptr && (*characteriter)->CollisionObject(*buildingiter)) {
				(*characteriter).reset();
				ClearDeletedObject(m_lArraowObjects, OBJECT_ARROW);
				m_pSound->PlaySoundW(m_iBoomIndex, false, 1.0f);
				break;
			}
		}

		for (bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end();) {
			if ((*bulletiter) == nullptr) {
				bulletiter++;
				continue;
			}

			if ((*characteriter) != nullptr && (*characteriter)->CollisionObject(*bulletiter)) {
				(*characteriter).reset();
				m_pSound->PlaySoundW(m_iBoomIndex, false, 1.0f);
				ClearDeletedObject(m_lArraowObjects, OBJECT_ARROW);
				break;
			}

			else bulletiter++;
		}

		for (arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); arrowiter++)
			if ((*arrowiter) != nullptr && (*arrowiter)->CollisionObject(*characteriter)) {
				(*arrowiter).reset();
				m_pSound->PlaySoundW(m_iBoomIndex, false, 1.0f);
			}
	}
		
	// 건물 -> 화살, 총알 
	for (buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); buildingiter++) {
		if (*buildingiter == nullptr) continue;

		for (bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end(); bulletiter++)
			if ((*bulletiter) != nullptr && (*bulletiter)->CollisionObject(*buildingiter)) {
				(*bulletiter).reset();
				m_pSound->PlaySoundW(m_iBoomIndex, false, 1.0f);
			}

		for (arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); arrowiter++)
			if ((*arrowiter) != nullptr && (*arrowiter)->CollisionObject(*buildingiter)) {
				(*arrowiter).reset();
				m_pSound->PlaySoundW(m_iBoomIndex, false, 1.0f);
			}
	}

	ClearDeletedObject(m_lArraowObjects, OBJECT_ARROW);
	ClearDeletedObject(m_lBuildingObjects, OBJECT_BUILDING);
	ClearDeletedObject(m_lCharacterObjects, OBJECT_CHARACTER);
	ClearDeletedObject(m_lBulletObjects, OBJECT_BULLET);


}

void SceneManager::Render()
{

	m_pRenderer->DrawTexturedRect(0.0f, 0.0f, 0.0f , WIN_HEIGHT, 1.0f , 1.0f , 1.0f , 1.0f , m_iTexID[4], LEVEL_UNDERGROUND);
	m_pRenderer->DrawParticleClimate(0, 0, 0, 5, 1, 1, 1, 1, -0.5f, -0.5f, m_iTexID[5], m_fSnowTime, 0.01);

	for (auto characiter = m_lCharacterObjects.begin(); characiter != m_lCharacterObjects.end(); ++characiter)		(*characiter)->Render();
	for (auto buildingiter = m_lBuildingObjects.begin(); buildingiter != m_lBuildingObjects.end(); ++buildingiter)	(*buildingiter)->Render();
	for (auto arrowiter = m_lArraowObjects.begin(); arrowiter != m_lArraowObjects.end(); ++arrowiter)				(*arrowiter)->Render();
	for (auto bulletiter = m_lBulletObjects.begin(); bulletiter != m_lBulletObjects.end(); ++bulletiter)			(*bulletiter)->Render();

}

int SceneManager::AddCharacter(Vector3& vec3Pos, TeamType type)
{ 
	if (type == TEAM_1 && m_fTeam1CharacterCooldown < 1.0f) return -1;
	if (type == TEAM_2 && m_fTeam2CharacterCooldown < 2.0f) return -1;

	CreateNewObject(vec3Pos, OBJECT_CHARACTER, type);
	if (type == TEAM_1) m_fTeam1CharacterCooldown = 0.0f;
	if (type == TEAM_2) m_fTeam2CharacterCooldown = 0.0f;
		
	return 1;
}

std::shared_ptr<Object>* SceneManager::CreateNewObject(Vector3& pos, ObjectType type, TeamType team)
{
	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> ui(-250, 250);

	Vector3 vecDirection;
	Vector4 vecColor;
	float fsize = 0.0f;
	float fValocity = 0.0f;
	float fLife = 0.0f;
	float fLevel = LEVEL_GOD;
	int texID = 0;

	switch (type) {
	case OBJECT_BUILDING:
		vecColor	 = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		fValocity	 = 0.0f;
		fsize		 = 100.0f;
		fLife		 = 500.0f;
		vecDirection = Vector3(0.0f, 0.0f, 0.0f);
		fLevel		 = LEVEL_BUILDING;
		texID		 = team == TEAM_1 ? m_iTexID[0] : m_iTexID[1];
		break;
	case OBJECT_CHARACTER:
		fValocity	 = 300.0f;
		fsize		 = 30.0f;
		fLife		 = 100.0f;
		vecColor	 = team == TEAM_1 ? Vector4(1.0f, 0.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		fLevel		 = LEVEL_CHARACTER;
		texID		 = m_iTexID[2];
		break;
	case OBJECT_BULLET:
		fValocity	 = 300.0f;
		fsize		 = 4.0f;
		fLife		 = 15.0f;
		vecColor	 = team == TEAM_1 ? Vector4(1.0f, 0.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		fLevel		 = LEVEL_BULLET;
		texID		 = m_iTexID[3];
		break;
	case OBJECT_ARROW:
		fValocity	 = 100.0f;
		fsize		 = 4.0f;
		fLife		 = 15.0f;
		vecColor	 = team == TEAM_1 ? Vector4(0.5f, 0.2f, 0.7f, 1.0f) : Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		fLevel = LEVEL_ARROW;
		break;
	}

	if (!IsFull()) {
		std::shared_ptr<Object> pNewObject(new Object(pos, fsize, vecColor, m_pRenderer, vecDirection, fValocity, type, team, fLife , texID ,fLevel));
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

void SceneManager::InitSceneManager()
{
	m_pRenderer = new Renderer(WIN_WIDTH, WIN_HEIGHT);
	m_iTexID = new int[6];
		m_iTexID[0] = m_pRenderer->CreatePngTexture("./Textures/PNGs/Hormor.png");
		m_iTexID[1] = m_pRenderer->CreatePngTexture("./Textures/PNGs/Bart.png");
		m_iTexID[2] = m_pRenderer->CreatePngTexture("./Textures/PNGs/squirrelSprite.png");
		m_iTexID[3] = m_pRenderer->CreatePngTexture("./Textures/PNGs/flare.png");
		m_iTexID[4] = m_pRenderer->CreatePngTexture("./Textures/PNGs/BackGround.png");
		m_iTexID[5] = m_pRenderer->CreatePngTexture("./Textures/PNGs/Snow.png");

	m_sScreen = Screen(WIN_WIDTH, WIN_HEIGHT);
	m_tTime = Time();
	m_pSound = new Sound();

	m_iBGMIndex = m_pSound->CreateSound("./Dependencies/SoundSamples/ophelia.mp3");
	m_pSound->PlaySoundW(m_iBGMIndex, true, 1.0f);

	m_iBoomIndex = m_pSound->CreateSound("./Dependencies/SoundSamples/explosion.WAV");

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
	m_tTime.Update(100.0);
	float fElapsedTime = m_tTime.Tick();

	Update(fElapsedTime);
	Render();
	
	std::string titlename = m_tTime.GetFrameTime();
	glutSetWindowTitle(titlename.c_str());
	m_tTime.Tock();
}