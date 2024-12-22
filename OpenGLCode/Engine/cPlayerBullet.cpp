#include "cPlayerBullet.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cPlayerBullet::cPlayerBullet():
	m_BulletMesh(nullptr),
	m_BulletSpeed(glm::vec3(0.0f))
{
	
}

cPlayerBullet::cPlayerBullet(glm::vec3 initialPosition, float speed, std::string playerBulletMesh)
{
	m_BulletMesh = new cMesh();
	m_BulletMesh->meshName = playerBulletMesh;
	m_BulletMesh->setDrawOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    m_BulletMesh->isUsingVertexColors = true;
    m_BulletMesh->bUseDebugColours = false;
    m_BulletMesh->bDoNotLight = true;
    m_BulletMesh->drawPosition = initialPosition;
	m_BulletSpeed = glm::vec3(0.0f, speed, 0.0f);
}

cPlayerBullet::~cPlayerBullet()
{
	delete m_BulletMesh;
}

void cPlayerBullet::UpdatePosition(double deltaTime)
{
	m_BulletMesh->drawPosition.x += (float)(m_BulletSpeed.x * deltaTime);
	m_BulletMesh->drawPosition.y += (float)(m_BulletSpeed.y * deltaTime);
	m_BulletMesh->drawPosition.z += (float)(m_BulletSpeed.z * deltaTime);

	if (m_BulletMesh->drawPosition.y > m_MaxY || m_IsBulletUsed)
	{
		g_currentScene->RemoveBullet(this);
		g_CollisionMediator->RemoveBullet(this);
	}
}

cMesh* cPlayerBullet::GetBulletMesh()
{
	return m_BulletMesh;
}

bool cPlayerBullet::IsBulletUsed()
{
	return m_IsBulletUsed;
}

void cPlayerBullet::BulletHit()
{
	m_IsBulletUsed = true;
}

void cPlayerBullet::SetBulletSpeed(glm::vec3 bulletSpeed)
{
	m_BulletSpeed = bulletSpeed;
}

std::string cPlayerBullet::GetBulletType()
{
	return "playerBullet";
}
