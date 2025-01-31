#include "cBeeEnemy.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cBeeEnemy::cBeeEnemy()
{
	m_EnemyMesh = new cMesh("beeEnemy");
	m_EnemyMesh->meshName = "beeEnemy.ply";
	m_EnemyMesh->bDoNotLight = true;
	m_EnemyMesh->isUsingVertexColors = true;
	m_EnemyMesh->bUseDebugColours = false;

	m_EnemyManager = cEnemyManager(this, "bee", m_EnemyMesh, glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(0.0f, -1000.0f, 0.0f), 20.0);
	m_EnemyManager.SelectMovementType(cEnemyManager::movementType::Sine);
}

cBeeEnemy::~cBeeEnemy()
{

}

void cBeeEnemy::ShootBullet()
{
	bulletFactory.CreateBullet(m_EnemyMesh->drawPosition.x , m_EnemyMesh->drawPosition.y, m_EnemyMesh->drawPosition.z);
}

void cBeeEnemy::SetHealth(int health)
{
	m_Health = health;
}

void cBeeEnemy::DamageEnemy(int damage)
{
	m_Health -= damage;

	if (m_Health <= 0)
	{
		m_IsDead = true;
		m_Health = 0;
		DestroyEnemy();
	}
}

void cBeeEnemy::DestroyEnemy()
{
	g_CollisionMediator->RemoveEnemy(this);
}

void cBeeEnemy::UpdatePosition(double deltaTime)
{
	m_EnemyManager.Update(deltaTime);
}

std::string cBeeEnemy::GetEnemyType()
{
	return "bee";
}

void cBeeEnemy::Update(double deltaTime)
{
	UpdatePosition(deltaTime);

	timer -= deltaTime;

	if (timer < 0.0)
	{
		timer = 1.0;
	}

	if (m_CanShootBullet)
	{
		m_BulletTimer += deltaTime;

		if (m_BulletTimer > m_BulletCooldown)
		{
			ShootBullet();
			m_CanShootBullet = false;
		}
	}
}

bool cBeeEnemy::IsDead()
{
	return m_IsDead;
}

int cBeeEnemy::GetPoints()
{
	return m_EnemyPoints;
}
