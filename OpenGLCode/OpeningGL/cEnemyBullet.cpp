#include "cEnemyBullet.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cEnemyBullet::cEnemyBullet() :
	m_BulletMesh(nullptr),
	m_BulletSpeed(glm::vec3(0.0f))
{

}

cEnemyBullet::cEnemyBullet(glm::vec3 initialPosition, glm::vec3 velocity, std::string enemyBulletMesh)
{
	m_BulletMesh = new cMesh();
	m_BulletMesh->meshName = enemyBulletMesh;
	m_BulletMesh->setDrawOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	m_BulletMesh->isUsingVertexColors = true;
	m_BulletMesh->bUseDebugColours = false;
	m_BulletMesh->bDoNotLight = true;
	m_BulletMesh->drawPosition = initialPosition;
	m_BulletSpeed = velocity;
}

cEnemyBullet::~cEnemyBullet()
{
	delete m_BulletMesh;
}

void cEnemyBullet::UpdatePosition(double deltaTime)
{
	m_BulletMesh->drawPosition.x += (float)(m_BulletSpeed.x * deltaTime);
	m_BulletMesh->drawPosition.y += (float)(m_BulletSpeed.y * deltaTime);
	m_BulletMesh->drawPosition.z += (float)(m_BulletSpeed.z * deltaTime);

	if (m_BulletMesh->drawPosition.y < m_MinY)
	{
		g_CollisionMediator->RemoveBullet(this);
	}
}

cMesh* cEnemyBullet::GetBulletMesh()
{
	return m_BulletMesh;
}

bool cEnemyBullet::IsBulletUsed()
{
	return m_IsBulletUsed;
}

void cEnemyBullet::BulletHit()
{
	m_IsBulletUsed = true;
}

void cEnemyBullet::SetBulletSpeed(glm::vec3 bulletSpeed)
{
	m_BulletSpeed = bulletSpeed;
}

std::string cEnemyBullet::GetBulletType()
{
	return "enemyBullet";
}

