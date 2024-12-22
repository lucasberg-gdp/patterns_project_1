#include "cEnemyDirector.h"

cEnemyDirector::cEnemyDirector(cEnemyBuilder* enemyBuilder)
{
    m_EnemyBuilder = enemyBuilder;
}

iEnemy* cEnemyDirector::CreateBeeEnemy()
{
    m_EnemyBuilder->StartNewEnemy();
    m_EnemyBuilder->CreateBeeEnemy();
    m_EnemyBuilder->SetHealth();

    return m_EnemyBuilder->GetEnemy();
}

iEnemy* cEnemyDirector::CreateButterflyEnemy()
{
    m_EnemyBuilder->StartNewEnemy();
    m_EnemyBuilder->CreateButterflyEnemy();
    m_EnemyBuilder->SetHealth();

    return m_EnemyBuilder->GetEnemy();
}

iEnemy* cEnemyDirector::CreateMothEnemy()
{
    m_EnemyBuilder->StartNewEnemy();
    m_EnemyBuilder->CreateMothEnemy();
    m_EnemyBuilder->SetHealth();

    return m_EnemyBuilder->GetEnemy();
}

iEnemy* cEnemyDirector::CreateBeeEnemy(glm::vec3 enemyPosition)
{
    m_EnemyBuilder->StartNewEnemy();
    m_EnemyBuilder->CreateBeeEnemy();
    m_EnemyBuilder->SetHealth();
    m_EnemyBuilder->SetEnemyPosition(enemyPosition);
    return m_EnemyBuilder->GetEnemy();
}

iEnemy* cEnemyDirector::CreateButterflyEnemy(glm::vec3 enemyPosition)
{
    m_EnemyBuilder->StartNewEnemy();
    m_EnemyBuilder->CreateButterflyEnemy();
    m_EnemyBuilder->SetHealth();
    m_EnemyBuilder->SetEnemyPosition(enemyPosition);

    return m_EnemyBuilder->GetEnemy();
}

iEnemy* cEnemyDirector::CreateMothEnemy(glm::vec3 enemyPosition)
{
    m_EnemyBuilder->StartNewEnemy();
    m_EnemyBuilder->CreateMothEnemy();
    m_EnemyBuilder->SetHealth();
    m_EnemyBuilder->SetEnemyPosition(enemyPosition);

    return m_EnemyBuilder->GetEnemy();
}
