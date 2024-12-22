#include "cMothEnemy.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cMothEnemy::cMothEnemy() :
    m_MovementType(cEnemyManager::movementType::Sine)
{
    m_EnemyMesh = new cMesh("mothEnemy");
    m_EnemyMesh->meshName = m_GreenMoth;
    m_EnemyMesh->bDoNotLight = true;
    m_EnemyMesh->isUsingVertexColors = true;
    m_EnemyMesh->bUseDebugColours = false;

    m_EnemyManager = cEnemyManager("moth", m_EnemyMesh, glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(0.0f, -1000.0f, 0.0f), 20.0f);
    m_EnemyManager.SelectMovementType(cEnemyManager::movementType::Sine);
    m_EnemyManager.SetRightToLeftIntroMovement();
}

cMothEnemy::~cMothEnemy()
{

}

void cMothEnemy::ShootBullet()
{
    bulletFactory.CreateBullet(m_EnemyMesh->drawPosition.x, m_EnemyMesh->drawPosition.y, m_EnemyMesh->drawPosition.z);
}

void cMothEnemy::SetHealth(int health)
{
    m_Health = health;
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

cMesh* cMothEnemy::GetEnemyMesh()
{
    return m_EnemyMesh;
}

void cMothEnemy::Attack()
{
    m_IsAttacking = true;
    m_EnemyManager.StartMoving(m_EnemyMesh->drawPosition);
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

void cMothEnemy::SetIntroType(std::string introType)
{
    if (introType == "basicHalfCircle")
    {
        m_EnemyManager.SetLeftToRightIntroMovement();
    }
    else if (introType == "invertedBasicHalfCircle")
    {
        m_EnemyManager.SetLeftToRightIntroMovement();
    }
    else if (introType == "basicFullCircle")
    {
        m_EnemyManager.SetLeftToRightFullCircleIntroMovement();
    }
    else if (introType == "invertedBasicFullCircle")
    {
        m_EnemyManager.SetRightToLeftFullCircleIntroMovement();
    }
}

void cMothEnemy::Skirmish()
{
    m_IsSkirmishing = true;
    m_IsInGrid = false;
    m_EnemyManager.StartSkirmishing();
    m_CanShootBullet = true;
}

bool cMothEnemy::IsDead()
{
    return m_IsDead;
}

int cMothEnemy::GetPoints()
{
    return m_EnemyPoints;
}
