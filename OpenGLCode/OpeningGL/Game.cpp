#include <sstream>

#include "Game.h"
#include "cGameManager.h"
#include "Character.h"

#include "cColor.h"
#include "cAnimationSystem.h"
#include "Engine.h"
#include "cSoftBodyVerlet.h"
#include "MathUtils.h"
#include "cEnemyManager.h"
#include <glm/gtc/type_ptr.hpp>
#include "cParticleSystem.h"
#include "VegetationGenerator.h"
#include "Basic Shader Manager/cShaderManager.h"
#include "cCube.h"
#include "cStars.h"
#include "cEnemyBuilder.h"
#include "cEnemyDirector.h"
#include "cCollisionMediator.h"

// TODO: Remove this
#include "cEnemyBulletFactory.h"
#include "cGameGrid.h"
    
extern cScene* g_currentScene;
extern cVAOManager* g_pMeshManager;
extern GLuint shaderProgramID;
extern cAnimationSystem* g_animationSystem;
extern cSoftBodyVerlet* g_softBody;
extern Engine engine;
extern cShaderManager* shaderManager;

extern std::vector< cSoftBodyVerlet* > chainSoftBodies;

cMesh* plank = nullptr;

extern cMesh* g_particleMeshModel;

extern cParticleSystem g_anEmitter;
extern cMesh* g_rainMeshModel;
extern cMesh* islandTerrain;
extern cParticleSystem* rainParticleSystem;

extern cMesh* g_rainSplashMesh;

extern cCollisionMediator* g_CollisionMediator;

extern cStars* stars;

cEnemyBuilder builder;

std::vector<iEnemy*> leftSquad;
std::vector<iEnemy*> rightSquad;
std::vector<iEnemy*> bottomLeftSquad;



void Game::CreateStageSquads()
{
    // First squad
    cSquad firstSquad = cSquad(4,2,true);

    firstSquad.SetBeesMovementType("basicHalfCircle");
    firstSquad.SetButterfliesMovementType("invertedBasicHalfCircle");

    squads.push_back(firstSquad);

    // Second squad
    cSquad secondSquad = cSquad();

    secondSquad.CreateButterflyAndMothsSquad(4, 1, true);
    secondSquad.SetButterfliesMovementType("basicFullCircle");
    secondSquad.SetMothsMovementType("basicFullCircle");

    squads.push_back(secondSquad);

    // Third squad
    cSquad thirdSquad = cSquad(0, 8, 0, 1);
    thirdSquad.SetButterfliesMovementType("invertedBasicFullCircle");
    squads.push_back(thirdSquad);

    // Fourth squad
    cSquad fourthSquad = cSquad(8, 0, 0, 1);
    fourthSquad.SetBeesMovementType("invertedBasicHalfCircle");
    squads.push_back(fourthSquad);

    // Fifth squad
    cSquad fifthSquad = cSquad(8, 0, 0, 1);
    fifthSquad.SetBeesMovementType("basicHalfCircle");
    squads.push_back(fifthSquad);

    // Sum all score
    for (int i = 0; i < squads.size(); i++)
    {
        for (int j = 0; j < squads[i].m_EnemiesInSquad.size(); j++)
        {
            g_currentScene->score->AddToTotalScore(squads[i].m_EnemiesInSquad[j]->GetPoints());
        }
    }

}

void Game::StartSkirmishing()
{
    isSkirmishing = true;
}

void Game::UpdateSkirmish(double deltaTime)
{
    if (g_currentScene->enemies.size() == 0)
    {
        isSkirmishing = false;
    }

    if (!isSkirmishing)
    {
        return;
    }

    skirmishTimer += deltaTime;

    if (skirmishTimer > skirmishCooldown)
    {
        skirmishTimer = 0.0;

        int randomEnemyIndex = MathUtils::GetRandomInt(0, (int)g_currentScene->enemies.size() - 1);
        g_currentScene->enemies[randomEnemyIndex]->Skirmish();
    }
}

void Game::SkirmishWithAllEnemies()
{
    for (unsigned int i = 0; i < g_currentScene->enemies.size(); i++)
    {
        g_currentScene->enemies[i]->Skirmish();
    }
}

void Game::StartGame()
{
    if (!isInMenu)
    {
        return;
    }

    isInMenu = false;

    g_currentScene->worldText->SetText("");
    g_currentScene->m_Player->InstantlyRevivePlayer();

    cMesh* playerShipMesh = g_currentScene->m_Player->GetPlayerMesh();
    playerShipMesh->bIsVisible = true;

    CreateStageSquads();
    g_currentScene->score->StartScoring();

    if (squads.size() > 0)
    {
        squads[currentShipIndex].m_IsSquadRunning = true;
    }

    g_CollisionMediator->SetActive(true);
}

void Game::PauseGame()
{
    isGamePaused = true;
}

void Game::RestartGame()
{
    if (!isGameOver)
    {
        return;
    }

    isGameOver = true;

    ResetGameTimers();

    gameStartDelayTimer = 0.0;
    g_currentScene->ClearStage();
    squads.clear();
    g_currentScene->gameGrid.ResetGameGrid();

    isStageComplete = false;
    isSkirmishing = false;

    currentShipIndex = 0;
    squadTimer = 0.0;
    skirmishTimer = 0;

    g_CollisionMediator->SetActive(true);

    g_currentScene->ClearStage();

    squads.clear();

    g_currentScene->worldText->SetText("");
    g_currentScene->m_Player->InstantlyRevivePlayer();
    g_CollisionMediator->RemoveAllEnemies();
    g_CollisionMediator->RemoveAllBullets();

    g_currentScene->score->RestartScore();
    CreateStageSquads();
    g_currentScene->score->StartScoring();

    if (squads.size() > 0)
    {
        squads[currentShipIndex].m_IsSquadRunning = true;
    }

    g_CollisionMediator->SetActive(true);
}

void Game::RetryLevel()
{
    if (!isStageComplete)
    {
        return;
    }

    isStageComplete = true;

    ResetGameTimers();

    gameStartDelayTimer = 0.0;
    g_currentScene->ClearStage();
    squads.clear();
    g_currentScene->gameGrid.ResetGameGrid();

    isSkirmishing = false;

    g_currentScene->worldText->SetText("");
    g_currentScene->m_Player->InstantlyRevivePlayer();
    g_CollisionMediator->RemoveAllEnemies();
    g_CollisionMediator->RemoveAllBullets();

    g_currentScene->score->RestartScore();
    CreateStageSquads();
    g_currentScene->score->StartScoring();

    currentShipIndex = 0;
    squadTimer = 0.0; 
    skirmishTimer = 0;

    if (squads.size() > 0)
    {
        squads[currentShipIndex].m_IsSquadRunning = true;
    }

    g_CollisionMediator->SetActive(true);

    isGameOver = false;
}

void Game::RevivePlayer()
{

}

void Game::ResetGameTimers()
{
    squadTimer = 0.0;
    skirmishTimer = 0.0;
    timerForNextSquad = -5.0;
    gameStartDelayTimer = 0.0;
}

bool Game::IsGameOver()
{
    return isGameOver;
}

bool Game::IsStageComplete()
{
    return isStageComplete;
}

void Game::UpdateCurrentSquad(double deltaTime)
{
    if (squads.size() == 0)
    {
        return;
    }

    gameStartDelayTimer += deltaTime;

    if (gameStartDelayTimer > gameStartDelay)
    {
        squads[currentShipIndex].Update(deltaTime);

        if (squads[currentShipIndex].m_IsSquadDone)
        {
            timerForNextSquad += deltaTime;

            if (timerForNextSquad > 5.0f)
            {
                SendNextSquad();
                timerForNextSquad = 0.0;
            }
        }
    }
}

Game::Game()
{

}

void Game::SendNextSquad()
{
    if (currentShipIndex + 1 < squads.size())
    {
        ++currentShipIndex;
        squads[currentShipIndex].m_IsSquadRunning = true;
    }
    else
    {
        isSkirmishing = true;
    }
}

void Game::Awake(GLFWwindow& window)
{

}

void Game::Start(GLFWwindow& window)
{
    g_CollisionMediator = new cCollisionMediator();

    g_currentScene->m_Player = new cPlayer(3);
    g_currentScene->m_Player->SetPlayerSpeed(1300.0);
    cMesh* playerShipMesh = g_currentScene->m_Player->GetPlayerMesh();
    playerShipMesh->bIsVisible = false;
    g_currentScene->AddMesh(playerShipMesh);

    g_currentScene->m_Player->DisablePlayer();
    g_CollisionMediator->SetPlayer(g_currentScene->m_Player);

    stars = new cStars(250, -200.0f);

    g_currentScene->gameGrid = cGameGrid(glm::vec3(0.0f, 200.0f, 0.0f));

    g_currentScene->StartScene();

    g_currentScene->score = new cScore();

    g_currentScene->worldText = new cWorldText();

    //cEnemyDirector director = cEnemyDirector(&builder);

    //iEnemy* beeEnemy = director.CreateBeeEnemy(glm::vec3(-1000.0f, 800.0f, 0.0f));
    //iEnemy* butterflyEnemy = director.CreateButterflyEnemy(glm::vec3(0.0f, 800.0f, 0.0f));
    //iEnemy* mothEnemy = director.CreateMothEnemy(glm::vec3(1000.0f, 800.0f, 0.0f));

    //cMesh* beeMesh = beeEnemy->GetEnemyMesh();
    //cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
    //cMesh* mothMesh = mothEnemy->GetEnemyMesh();

    //g_currentScene->AddMesh(beeMesh);
    //g_currentScene->AddMesh(butterflyMesh);
    //g_currentScene->AddMesh(mothMesh);

    //g_currentScene->enemies.push_back(beeEnemy);
    //g_currentScene->enemies.push_back(butterflyEnemy);
    //g_currentScene->enemies.push_back(mothEnemy);

    //g_CollisionMediator->AddEnemy(beeEnemy);
    //g_CollisionMediator->AddEnemy(butterflyEnemy);
    //g_CollisionMediator->AddEnemy(mothEnemy);

    // Testing explosions
    //cMesh* explosion1 = new cMesh("explosion");
    //cMesh* explosion2 = new cMesh("explosion");
    //cMesh* explosion3 = new cMesh("explosion");
    //cMesh* explosion4 = new cMesh("explosion");
    //cMesh* explosion5 = new cMesh("explosion");

    //explosion1->meshName = "explosion1.ply";
    //explosion2->meshName = "explosion2.ply";
    //explosion3->meshName = "explosion3.ply";
    //explosion4->meshName = "explosion4.ply";
    //explosion5->meshName = "explosion5.ply";

    //explosion1->bDoNotLight = true;
    //explosion2->bDoNotLight = true;
    //explosion3->bDoNotLight = true;
    //explosion4->bDoNotLight = true;
    //explosion5->bDoNotLight = true;

    //explosion1->isUsingVertexColors = true;
    //explosion2->isUsingVertexColors = true;
    //explosion3->isUsingVertexColors = true;
    //explosion4->isUsingVertexColors = true;
    //explosion5->isUsingVertexColors = true;

    ////m_EnemyMesh->bUseDebugColours = false;

    //explosion1->drawPosition = glm::vec3(500.0f, 800.0f, 0.0f);
    //explosion2->drawPosition = glm::vec3(250.0f, 800.0f, 0.0f);
    //explosion3->drawPosition = glm::vec3(0.0f, 800.0f, 0.0f);
    //explosion4->drawPosition = glm::vec3(-250.0f, 800.0f, 0.0f);
    //explosion5->drawPosition = glm::vec3(-500.0f, 800.0f, 0.0f);

    //g_currentScene->AddMesh(explosion1);
    //g_currentScene->AddMesh(explosion2);
    //g_currentScene->AddMesh(explosion3);
    //g_currentScene->AddMesh(explosion4);
    //g_currentScene->AddMesh(explosion5);


    // Testing Enemy bullets
    //cEnemyBulletFactory enemyBulletFactor;

    //iBullet* enemyBullet = enemyBulletFactor.CreateBullet(glm::vec3(-1000.0f, 500.0f, 0.0f));

    //cMesh* enemyBulletMesh = enemyBullet->GetBulletMesh();

    //g_currentScene->AddMesh(enemyBulletMesh);

    // Movement

    //c2DNavigation navigation;

    //std::vector<glm::vec3> positions = navigation.BeeIntroPositions();

    //SendFirstSquad();


}

void Game::EarlyUpdate(GLFWwindow& window, double deltaTime)
{

}

void Game::Update(GLFWwindow& window, double deltaTime)
{
    if (isGamePaused)
    {
        return;
    }

    g_CollisionMediator->Update(deltaTime);
    g_currentScene->m_Player->Update(deltaTime);
    stars->Update(deltaTime);

    for (iBullet* bullet : g_currentScene->sceneBullets) 
    {
        bullet->UpdatePosition(deltaTime);
    }

    for (iEnemy* enemy : g_currentScene->enemies)
    {
        enemy->Update(deltaTime);
    }

    g_currentScene->gameGrid.Update(deltaTime);
    g_currentScene->score->Update(deltaTime);
    g_currentScene->worldText->Update(deltaTime);

    // Update squads
    UpdateCurrentSquad(deltaTime);

    // Update skirmish
    UpdateSkirmish(deltaTime);

    if (g_currentScene->m_Player->IsDead())
    {
        isGameOver = true;
        
    }

    if (g_currentScene->score->IsStageComplete())
    {
        g_currentScene->worldText->SetText("stage complete");
        //isGameOver = true;
        isStageComplete = true;
    }
}

void Game::LateUpdate(GLFWwindow& window, double deltaTime)
{

}

void Game::End(GLFWwindow& window)
{

}

std::string Game::GetSystemType()
{
    return "Game";
}

void Game::SendFirstSquad()
{
    if (isFirstSquadRunning)
    {
        return;
    }

    isFirstSquadRunning = true;

    cEnemyDirector director = cEnemyDirector(&builder);

    //Four bees right to left
    int numberOfBees = 4;
    for (int i = 0; i < numberOfBees; i++)
    {
        iEnemy* beeEnemy = director.CreateBeeEnemy();
        cMesh* beeMesh = beeEnemy->GetEnemyMesh();
        beeMesh->bIsVisible = false;
        g_currentScene->AddMesh(beeMesh);
        g_currentScene->enemies.push_back(beeEnemy);
        g_CollisionMediator->AddEnemy(beeEnemy);
        rightSquad.push_back(beeEnemy);
    }

    //Four butterflies
    int numberOfButterflies = 4;
    for (int i = 0; i < numberOfButterflies; i++)
    {
        iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
        cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
        butterflyMesh->bIsVisible = false;
        g_currentScene->AddMesh(butterflyMesh);
        g_currentScene->enemies.push_back(butterflyEnemy);
        g_CollisionMediator->AddEnemy(butterflyEnemy);
        leftSquad.push_back(butterflyEnemy);
    }
}

void Game::SendSecondSquad()
{
    if (isSecondSquadRunning)
    {
        return;
    }

    isSecondSquadRunning = true;

    cEnemyDirector director = cEnemyDirector(&builder);

    //Four butterflies
    int numberOfButterflies = 4;
    for (int i = 0; i < numberOfButterflies; i++)
    {
        iEnemy* butterflyEnemy = director.CreateButterflyEnemy();
        cMesh* butterflyMesh = butterflyEnemy->GetEnemyMesh();
        butterflyEnemy->SetIntroType("basicFullCircle");
        butterflyMesh->bIsVisible = false;
        g_currentScene->AddMesh(butterflyMesh);
        g_currentScene->enemies.push_back(butterflyEnemy);
        g_CollisionMediator->AddEnemy(butterflyEnemy);
        bottomLeftSquad.push_back(butterflyEnemy);

        iEnemy* mothEnemy = director.CreateMothEnemy();
        mothEnemy->SetIntroType("basicFullCircle");
        cMesh* mothMesh = mothEnemy->GetEnemyMesh();
        mothMesh->bIsVisible = false;
        g_currentScene->AddMesh(mothMesh);
        g_currentScene->enemies.push_back(mothEnemy);
        g_CollisionMediator->AddEnemy(mothEnemy);
        bottomLeftSquad.push_back(mothEnemy);
    }
}