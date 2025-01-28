#include "cEnemy.h"

void cEnemy::SetIntroType(std::string introType)
{
    if (introType == "bezierIntro")
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
    else if (introType == "bezierInvertedIntroFullCircle")
    {
        m_EnemyManager.SetBezierInvertedIntroLeftRightMovement();
    }
}

void cEnemy::Attack()
{
    m_IsAttacking = true;
    m_EnemyManager.StartMoving(m_EnemyMesh->drawPosition);
}

void cEnemy::Skirmish()
{
    m_IsSkirmishing = true;
    m_IsInGrid = false;
    m_EnemyManager.StartSkirmishing();
    m_CanShootBullet = true;
}

bool cEnemy::IsDead()
{
    return m_IsDead;
}

int cEnemy::GetPoints()
{
    return m_EnemyPoints;
}

void cEnemy::SetHealth(int health)
{
    m_Health = health;
}

cMesh* cEnemy::GetEnemyMesh()
{
    return m_EnemyMesh;
}

bool cEnemy::IsInGrid()
{
    return m_IsInGrid;
}

void cEnemy::SetIsInGrid(bool isInGrid)
{
    m_IsInGrid = isInGrid;
}