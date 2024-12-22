#pragma once
#include "iEnemy.h"

class cEnemyBuilder
{
    iEnemy* m_EnemyBeingBuilt;
public:
    void StartNewEnemy();
    void SetHealth();
    void SetEnemyPosition(glm::vec3 enemyPosition);
    void SetIntroMovement(std::string introMovement);

    void CreateBeeEnemy();
    void CreateButterflyEnemy();
    void CreateMothEnemy();

    iEnemy* GetEnemy();
};

