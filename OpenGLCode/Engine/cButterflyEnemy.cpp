#include "cButterflyEnemy.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cButterflyEnemy::cButterflyEnemy():
	m_MovementType(cEnemyManager::movementType::Sine)
{
    m_EnemyMesh = new cMesh("butterflyEnemy");
    m_EnemyMesh->meshName = "butterflyEnemy.ply";
    m_EnemyMesh->bDoNotLight = true;
    m_EnemyMesh->isUsingVertexColors = true;
    m_EnemyMesh->bUseDebugColours = false;

    m_EnemyManager = cEnemyManager("butterfly", m_EnemyMesh, glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(0.0f, -1000.0f, 0.0f), 20.0f);
    m_EnemyManager.SelectMovementType(cEnemyManager::movementType::Sine);
    m_EnemyManager.SetRightToLeftIntroMovement();
}

void cButterflyEnemy::ShootBullet()
{
    bulletFactory.CreateBullet(m_EnemyMesh->drawPosition.x, m_EnemyMesh->drawPosition.y, m_EnemyMesh->drawPosition.z);
}

void cButterflyEnemy::SetHealth(int health)
{
    m_Health = health;
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

cMesh* cButterflyEnemy::GetEnemyMesh()
{
    return m_EnemyMesh;
}

void cButterflyEnemy::Attack()
{
    m_IsAttacking = true;
    m_EnemyManager.StartMoving(m_EnemyMesh->drawPosition);
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

void cButterflyEnemy::SetIntroType(std::string introType)
{
    if (introType == "basicHalfCircle")
    {
        m_EnemyManager.SetLeftToRightIntroMovement();
    }
    else if (introType == "invertedBasicHalfCircle")
    {
        m_EnemyManager.SetRightToLeftIntroMovement();
    }
    else if (introType == "basicFullCircle")
    {
        m_EnemyManager.SetLeftToRightFullCircleIntroMovement();
    }
    else if (introType == "invertedBasicFullCircle")
    {
        m_EnemyManager.SetRightToLeftFullCircleIntroMovement();
    }
    else if (introType == "bezierIntro")
    {
        m_EnemyManager.SetBezierIntroMovement();
    }
    else if (introType == "bezierInvertedIntro")
    {
        m_EnemyManager.SetInvertedBezierIntroMovement();
    }
    else if (introType == "bezierIntroFullCircle")
    {
        m_EnemyManager.SetBezierIntroLeftRightMovement();
    }
}

void cButterflyEnemy::Skirmish()
{
    m_IsSkirmishing = true;
    m_IsInGrid = false;
    m_EnemyManager.StartSkirmishing();
    m_CanShootBullet = true;
}

bool cButterflyEnemy::IsDead()
{
    return m_IsDead;
}

int cButterflyEnemy::GetPoints()
{
    return m_EnemyPoints;
}
