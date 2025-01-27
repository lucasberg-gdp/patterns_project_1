#include "cMothEnemy.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cMothEnemy::cMothEnemy()
{
    m_EnemyMesh = new cMesh("mothEnemy");
    m_EnemyMesh->meshName = m_GreenMoth;
    m_EnemyMesh->bDoNotLight = true;
    m_EnemyMesh->isUsingVertexColors = true;
    m_EnemyMesh->bUseDebugColours = false;

    m_EnemyManager = cEnemyManager("moth", m_EnemyMesh, glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(0.0f, -1000.0f, 0.0f), 20.0f);
    m_EnemyManager.SelectMovementType(cEnemyManager::movementType::Sine);
}

cMothEnemy::~cMothEnemy()
{

}

void cMothEnemy::ShootBullet()
{
    bulletFactory.CreateBullet(m_EnemyMesh->drawPosition.x, m_EnemyMesh->drawPosition.y, m_EnemyMesh->drawPosition.z);
}

void cMothEnemy::DamageEnemy(int damage)
{
    m_Health -= damage;

    if (m_Health == 1)
    {
        m_EnemyMesh->meshName = m_BlueMoth;
    }

    if (m_Health <= 0)
    {
        m_IsDead = true;
        m_Health = 0;
        DestroyEnemy();
    }
}

void cMothEnemy::DestroyEnemy()
{
    g_CollisionMediator->RemoveEnemy(this);
    g_currentScene->DeleteMeshById(m_EnemyMesh->GetUniqueID());
}

void cMothEnemy::UpdatePosition(double deltaTime)
{
    m_EnemyManager.Update(deltaTime);
}

std::string cMothEnemy::GetEnemyType()
{
    return "moth";
}

void cMothEnemy::Update(double deltaTime)
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