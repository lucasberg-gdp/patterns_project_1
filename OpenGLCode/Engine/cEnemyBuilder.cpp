#include "cEnemyBuilder.h"

#include "cBeeEnemy.h"
#include "cButterflyEnemy.h"
#include "cMothEnemy.h"

void cEnemyBuilder::StartNewEnemy()
{
    m_EnemyBeingBuilt = nullptr;
}

void cEnemyBuilder::SetHealth()
{
    std::string enemyType = m_EnemyBeingBuilt->GetEnemyType();

    if (enemyType == "bee")
    {
        m_EnemyBeingBuilt->SetHealth(1);
    }
    else if (enemyType == "butterfly")
    {
        m_EnemyBeingBuilt->SetHealth(1);
    }
    else if (enemyType == "moth")
    {
        m_EnemyBeingBuilt->SetHealth(2);
    }
}

void cEnemyBuilder::SetEnemyPosition(glm::vec3 enemyPosition)
{
    m_EnemyBeingBuilt->GetEnemyMesh()->drawPosition = enemyPosition;
}

void cEnemyBuilder::SetIntroMovement(std::string introMovement)
{

}

void cEnemyBuilder::CreateBeeEnemy()
{
    m_EnemyBeingBuilt = new cBeeEnemy();
}

void cEnemyBuilder::CreateButterflyEnemy()
{
    m_EnemyBeingBuilt = new cButterflyEnemy();
}

void cEnemyBuilder::CreateMothEnemy()
{
    m_EnemyBeingBuilt = new cMothEnemy();
}

iEnemy* cEnemyBuilder::GetEnemy()
{
    return m_EnemyBeingBuilt;
}
