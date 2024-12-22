#include "cSquad.h"
#include "cScene.h"
#include "cEnemyDirector.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

cSquad::cSquad(int numberOfBees, int numberOfButterflies, int numberOfMoths, int shipsPerCoolDown):
	m_NumberOfBees(numberOfBees),
	m_NumberOfButterflies(numberOfButterflies),
	m_NumberOfMoths(numberOfMoths),
    m_ShipsPerCoolDown(shipsPerCoolDown)
{
    cEnemyBuilder builder;
    cEnemyDirector director = cEnemyDirector(&builder);

    //bees
    for (int i = 0; i < numberOfBees; i++)
    {
        iEnemy* beeEnemy = director.CreateBeeEnemy();
        cMesh* beeMesh = beeEnemy->GetEnemyMesh();
        beeMesh->bIsVisible = false;
        g_currentScene->AddMesh(beeMesh);
        g_currentScene->enemies.push_back(beeEnemy);
        g_CollisionMediator->AddEnemy(beeEnemy);

        m_EnemiesInSquad.push_back(beeEnemy);
    }

    //butterflies
    for (int i = 0; i < numberOfButterflies; i++)
    {
        iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
        cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
        butterflyMesh->bIsVisible = false;
        g_currentScene->AddMesh(butterflyMesh);
        g_currentScene->enemies.push_back(butterflyEnemy);
        g_CollisionMediator->AddEnemy(butterflyEnemy);

        m_EnemiesInSquad.push_back(butterflyEnemy);
    }

    //moths
    for (int i = 0; i < numberOfMoths; i++)
    {
        iEnemy* mothEnemy = director.CreateMothEnemy();
        cMesh* mothMesh = mothEnemy->GetEnemyMesh();
        mothMesh->bIsVisible = false;
        g_currentScene->AddMesh(mothMesh);
        g_currentScene->enemies.push_back(mothEnemy);
        g_CollisionMediator->AddEnemy(mothEnemy);

        m_EnemiesInSquad.push_back(mothEnemy);
    }
}

cSquad::cSquad(int numberOfBees, int numberOfButterflies, int numberOfMoths, int shipsPerCoolDown, bool alternateEnemies) :
    m_NumberOfBees(numberOfBees),
    m_NumberOfButterflies(numberOfButterflies),
    m_NumberOfMoths(numberOfMoths),
    m_ShipsPerCoolDown(shipsPerCoolDown)
{
    cEnemyBuilder builder;
    cEnemyDirector director = cEnemyDirector(&builder);

    if (alternateEnemies)
    {

    }
    else
    {
        //bees
        for (int i = 0; i < numberOfBees; i++)
        {
            iEnemy* beeEnemy = director.CreateBeeEnemy();
            cMesh* beeMesh = beeEnemy->GetEnemyMesh();
            beeMesh->bIsVisible = false;
            g_currentScene->AddMesh(beeMesh);
            g_currentScene->enemies.push_back(beeEnemy);
            g_CollisionMediator->AddEnemy(beeEnemy);

            m_EnemiesInSquad.push_back(beeEnemy);
        }

        //butterflies
        for (int i = 0; i < numberOfButterflies; i++)
        {
            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);
        }

        //moths
        for (int i = 0; i < numberOfMoths; i++)
        {
            iEnemy* mothEnemy = director.CreateMothEnemy();
            cMesh* mothMesh = mothEnemy->GetEnemyMesh();
            mothMesh->bIsVisible = false;
            g_currentScene->AddMesh(mothMesh);
            g_currentScene->enemies.push_back(mothEnemy);
            g_CollisionMediator->AddEnemy(mothEnemy);

            m_EnemiesInSquad.push_back(mothEnemy);
        }
    }
}

cSquad::cSquad(int numberOfBeesAndButterflies, int shipsPerCoolDown, bool alternateEnemies):
    m_NumberOfBees(numberOfBeesAndButterflies),
    m_NumberOfButterflies(numberOfBeesAndButterflies),
    m_ShipsPerCoolDown(shipsPerCoolDown)
{
    cEnemyBuilder builder;
    cEnemyDirector director = cEnemyDirector(&builder);

    if (alternateEnemies)
    {
        for (int i = 0; i < numberOfBeesAndButterflies; i++)
        {
            iEnemy* beeEnemy = director.CreateBeeEnemy();
            cMesh* beeMesh = beeEnemy->GetEnemyMesh();
            beeMesh->bIsVisible = false;
            g_currentScene->AddMesh(beeMesh);
            g_currentScene->enemies.push_back(beeEnemy);
            g_CollisionMediator->AddEnemy(beeEnemy);

            m_EnemiesInSquad.push_back(beeEnemy);

            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);
        }
    }
    else
    {
        for (int i = 0; i < numberOfBeesAndButterflies; i++)
        {
            iEnemy* beeEnemy = director.CreateBeeEnemy();
            cMesh* beeMesh = beeEnemy->GetEnemyMesh();
            beeMesh->bIsVisible = false;
            g_currentScene->AddMesh(beeMesh);
            g_currentScene->enemies.push_back(beeEnemy);
            g_CollisionMediator->AddEnemy(beeEnemy);

            m_EnemiesInSquad.push_back(beeEnemy);
        }

        for (int i = 0; i < numberOfBeesAndButterflies; i++)
        {
            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);
        }
    }
}

void cSquad::CreateBeeAndButterflySquad(int numberOfBeesAndButterflies, int shipsPerCoolDown, bool alternateEnemies)
{
    cEnemyBuilder builder;
    cEnemyDirector director = cEnemyDirector(&builder);

    if (alternateEnemies)
    {
        for (int i = 0; i < numberOfBeesAndButterflies; i++)
        {
            iEnemy* beeEnemy = director.CreateBeeEnemy();
            cMesh* beeMesh = beeEnemy->GetEnemyMesh();
            beeMesh->bIsVisible = false;
            g_currentScene->AddMesh(beeMesh);
            g_currentScene->enemies.push_back(beeEnemy);
            g_CollisionMediator->AddEnemy(beeEnemy);

            m_EnemiesInSquad.push_back(beeEnemy);

            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);
        }
    }
    else
    {
        for (int i = 0; i < numberOfBeesAndButterflies; i++)
        {
            iEnemy* beeEnemy = director.CreateBeeEnemy();
            cMesh* beeMesh = beeEnemy->GetEnemyMesh();
            beeMesh->bIsVisible = false;
            g_currentScene->AddMesh(beeMesh);
            g_currentScene->enemies.push_back(beeEnemy);
            g_CollisionMediator->AddEnemy(beeEnemy);

            m_EnemiesInSquad.push_back(beeEnemy);
        }

        for (int i = 0; i < numberOfBeesAndButterflies; i++)
        {
            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);
        }
    }
}

void cSquad::CreateButterflyAndMothsSquad(int numberOfButterfliesAndMoths, int shipsPerCoolDown, bool alternateEnemies)
{
    cEnemyBuilder builder;
    cEnemyDirector director = cEnemyDirector(&builder);

    if (alternateEnemies)
    {
        for (int i = 0; i < numberOfButterfliesAndMoths; i++)
        {
            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);

            iEnemy* mothEnemy = director.CreateMothEnemy();
            cMesh* mothMesh = mothEnemy->GetEnemyMesh();
            mothMesh->bIsVisible = false;
            g_currentScene->AddMesh(mothMesh);
            g_currentScene->enemies.push_back(mothEnemy);
            g_CollisionMediator->AddEnemy(mothEnemy);

            m_EnemiesInSquad.push_back(mothEnemy);
        }
    }
    else
    {
        for (int i = 0; i < numberOfButterfliesAndMoths; i++)
        {
            iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
            cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
            butterflyMesh->bIsVisible = false;
            g_currentScene->AddMesh(butterflyMesh);
            g_currentScene->enemies.push_back(butterflyEnemy);
            g_CollisionMediator->AddEnemy(butterflyEnemy);

            m_EnemiesInSquad.push_back(butterflyEnemy);
        }

        for (int i = 0; i < numberOfButterfliesAndMoths; i++)
        {
            iEnemy* mothEnemy = director.CreateMothEnemy();
            cMesh* mothMesh = mothEnemy->GetEnemyMesh();
            mothMesh->bIsVisible = false;
            g_currentScene->AddMesh(mothMesh);
            g_currentScene->enemies.push_back(mothEnemy);
            g_CollisionMediator->AddEnemy(mothEnemy);

            m_EnemiesInSquad.push_back(mothEnemy);
        }
    }
}

void cSquad::Update(double deltaTime)
{
    if (!m_IsSquadDone && m_IsSquadRunning)
    {
        timer += deltaTime;

        if (timer >= timeForNextShip)
        {
            for (int i = 0; i < m_ShipsPerCoolDown; i++)
            {
                m_EnemiesInSquad[m_CurrentShipIndex]->Attack();
                m_EnemiesInSquad[m_CurrentShipIndex]->GetEnemyMesh()->bIsVisible = true;

                timer = 0.0;
                ++m_CurrentShipIndex;

                if (m_CurrentShipIndex >= m_EnemiesInSquad.size())
                {
                    m_IsSquadDone = true;
                    m_CurrentShipIndex = 0;
                }
            }
        }
    }
}

void cSquad::SetBeesMovementType(std::string introType)
{
    for (unsigned int i = 0; i < m_EnemiesInSquad.size(); i++)
    {
        if (m_EnemiesInSquad[i]->GetEnemyType() == "bee")
        {
            m_EnemiesInSquad[i]->SetIntroType(introType);
        }
    }
}

void cSquad::SetButterfliesMovementType(std::string introType)
{
    for (unsigned int i = 0; i < m_EnemiesInSquad.size(); i++)
    {
        if (m_EnemiesInSquad[i]->GetEnemyType() == "butterfly")
        {
            m_EnemiesInSquad[i]->SetIntroType(introType);
        }
    }
}

void cSquad::SetMothsMovementType(std::string introType)
{
    for (unsigned int i = 0; i < m_EnemiesInSquad.size(); i++)
    {
        if (m_EnemiesInSquad[i]->GetEnemyType() == "moth")
        {
            m_EnemiesInSquad[i]->SetIntroType(introType);
        }
    }
}

void cSquad::SortByShipType()
{
    std::sort(m_EnemiesInSquad.begin(), m_EnemiesInSquad.end(), [](iEnemy* a, iEnemy* b) 
    {
        return a->GetEnemyType() < b->GetEnemyType();
    });
}


