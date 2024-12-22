#pragma once
#include "cEnemyBuilder.h"

class cEnemyDirector
{
    cEnemyBuilder* m_EnemyBuilder = nullptr;
public:
    cEnemyDirector(cEnemyBuilder* enemyBuilder);

    iEnemy* CreateBeeEnemy();
    iEnemy* CreateButterflyEnemy();
    iEnemy* CreateMothEnemy();

    iEnemy* CreateBeeEnemy(glm::vec3 enemyPosition);
    iEnemy* CreateButterflyEnemy(glm::vec3 enemyPosition);
    iEnemy* CreateMothEnemy(glm::vec3 enemyPosition);
};

