#include "cButterflyEnemy.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cButterflyEnemy::cButterflyEnemy()
{
    m_EnemyMesh = new cMesh("butterflyEnemy");
    m_EnemyMesh->meshName = "butterflyEnemy.ply";
    m_EnemyMesh->bDoNotLight = true;
    m_EnemyMesh->isUsingVertexColors = true;
    m_EnemyMesh->bUseDebugColours = false;

    m_EnemyManager = cEnemyManager("butterfly", m_EnemyMesh, glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(0.0f, -1000.0f, 0.0f), 20.0f);
    m_EnemyManager.SelectMovementType(cEnemyManager::movementType::Sine);
}

void cButterflyEnemy::ShootBullet()
{
    bulletFactory.CreateBullet(m_EnemyMesh->drawPosition.x, m_EnemyMesh->drawPosition.y, m_EnemyMesh->drawPosition.z);
}

void cButterflyEnemy::DamageEnemy(int damage)
{
    m_Health -= damage;

    if (m_Health <= 0)
    {
        m_IsDead = true;
        m_Health = 0;
        DestroyEnemy();
    }
}

void cButterflyEnemy::DestroyEnemy()
{
    g_CollisionMediator->RemoveEnemy(this);
    g_currentScene->DeleteMeshById(m_EnemyMesh->GetUniqueID());
}

void cButterflyEnemy::UpdatePosition(double deltaTime)
{
    m_EnemyManager.Update(deltaTime);
}

std::string cButterflyEnemy::GetEnemyType()
{
    return "butterfly";
}

void cButterflyEnemy::Update(double deltaTime)
{
    UpdatePosition(deltaTime);

    timer -= deltaTime;

    if (timer < 0.0)
    {
        //ShootBullet();
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