#include <sstream>

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

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

#include "cLuaManager.h"
#include "cMoveRelativeTime.h"
#include "cOrientTo.h"
#include "cCamera.h"

#include "cGameGrid.h"
    
extern cScene* g_currentScene;
extern cVAOManager* g_pMeshManager;
extern GLuint shaderProgramID;
extern cAnimationSystem* g_animationSystem;
extern cSoftBodyVerlet* g_softBody;
extern Engine engine;
extern cShaderManager* shaderManager;

extern cMesh* g_particleMeshModel;

extern cParticleSystem g_anEmitter;
extern cMesh* g_rainMeshModel;
extern cMesh* islandTerrain;
extern cParticleSystem* rainParticleSystem;

extern cMesh* g_rainSplashMesh;

extern cCollisionMediator* g_CollisionMediator;

extern cStars* stars;

cEnemyBuilder builder;

void Game::CreateStageSquads()
{
    // First squad
    cSquad firstSquad = cSquad(4,2,true);

    firstSquad.SetBeesMovementType("bezierIntro");
    firstSquad.SetButterfliesMovementType("bezierInvertedIntro");

    squads.push_back(firstSquad);

    // Second squad
    cSquad secondSquad = cSquad();

    secondSquad.CreateButterflyAndMothsSquad(4, 1, true);
    secondSquad.SetButterfliesMovementType("bezierIntroFullCircle");
    secondSquad.SetMothsMovementType("bezierIntroFullCircle");

    squads.push_back(secondSquad);

    //// Third squad
    cSquad thirdSquad = cSquad(0, 8, 0, 1);
    thirdSquad.SetButterfliesMovementType("bezierInvertedIntroFullCircle");
    squads.push_back(thirdSquad);

    //// Fourth squad
    cSquad fourthSquad = cSquad(8, 0, 0, 1);
    fourthSquad.SetBeesMovementType("bezierIntroFullCircle");
    squads.push_back(fourthSquad);

    //// Fifth squad
    cSquad fifthSquad = cSquad(8, 0, 0, 1);
    fifthSquad.SetBeesMovementType("bezierInvertedIntroFullCircle");
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

        std::vector<iEnemy*> activeEnemies;

        for (unsigned int i = 0; i < g_currentScene->enemies.size(); i++)
        {
            if (g_currentScene->enemies[i]->IsInGrid())
            {
                iEnemy* activeEnemy = g_currentScene->enemies[i];
                activeEnemies.push_back(activeEnemy);
            }
        }

        if ((int)activeEnemies.size() > 0)
        {
            int randomEnemyIndex = MathUtils::GetRandomInt(0, (int)activeEnemies.size() - 1);
            activeEnemies[randomEnemyIndex]->Skirmish();
        }
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

    stars->IncreaseSpeed();

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

    stars->IncreaseSpeed();

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

    stars->IncreaseSpeed();

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

void Game::PressEnter()
{
    if (IsGameOver())
    {
        RestartGame();
        TransitionToBattleSong();
    }
    else if (IsStageComplete())
    {
        RetryLevel();
        TransitionToBattleSong();
    }
    else
    {
        StartGame();
        TransitionToBattleSong();
    }
}

void Game::ResetGameTimers()
{
    squadTimer = 0.0;
    skirmishTimer = 0.0;
    timerForNextSquad = -3.0;
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

            if (timerForNextSquad > 3.0f)
            {
                SendNextSquad();
                timerForNextSquad = 0.0;
            }
        }
    }
}

void Game::TransitionToBattleSong()
{
    isTransitioningSong = true;

    isSongTransitioningToBattle = true;
    isSongTransitioningToMenu = false;
}

void Game::TransitionToMenuSong()
{
    isTransitioningSong = true;

    isSongTransitioningToMenu = true;
    isSongTransitioningToBattle = false;
}

void Game::UpdateSounds(double deltaTime)
{
    std::string songPlaying = mediaPlayer->GetSongPlaying();

    if (isTransitioningSong)
    {
        if (isSongTransitioningToBattle)
        {
            float currentVolume = mediaPlayer->GetVolume();
            float newVolume = currentVolume - (float)(deltaTime * soundTransitionSpeed);

            mediaPlayer->SetVolume(newVolume);

            if (newVolume < 0.0f)
            {
                mediaPlayer->PlayAudio("battle.wav");
                isSongTransitioningToBattle = false;
            }
        }
        else if (isSongTransitioningToMenu && ("menu.wav" != songPlaying))
        {
            float currentVolume = mediaPlayer->GetVolume();
            float newVolume = currentVolume - (float)(deltaTime * soundTransitionSpeed);

            mediaPlayer->SetVolume(newVolume);

            if (newVolume < 0.0f)
            {
                mediaPlayer->PlayAudio("menu.wav");
                isSongTransitioningToMenu = false;
            }
        }
        else
        {
            float currentVolume = mediaPlayer->GetVolume();
            float newVolume = currentVolume + (float)(deltaTime * soundTransitionSpeed);

            if (newVolume <= 1.0f)
            {
                mediaPlayer->SetVolume(newVolume);
            }
            else
            {
                isTransitioningSong = false;
                isSongTransitioningToMenu = false;
                isSongTransitioningToBattle = false;
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
    mediaPlayer = new cMediaPlayer();
    mediaPlayer->Initialize();
    mediaPlayer->PlayAudio("menu.wav");
    mediaPlayer->SetLooping(true);

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
}

void Game::EarlyUpdate(GLFWwindow& window, double deltaTime)
{

}

void Game::Update(GLFWwindow& window, double deltaTime)
{
    UpdateSounds(deltaTime);

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
        stars->DecreaseSpeed();
        g_currentScene->worldText->SetText("stage complete");
        if (!isTransitioningSong)
        {
            TransitionToMenuSong();
        }
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

// Lua functions
int LuaAddSerialCommand(lua_State* L)
{
    std::string friendlyName = lua_tostring(L, 1);

    cGameObject* pGO = cLuaManager::FindObjectByFriendlyName(friendlyName);

    if (pGO == nullptr)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    std::vector<std::string> commandDetails;
    commandDetails.push_back(friendlyName);
    commandDetails.push_back(std::to_string(lua_tonumber(L, 3)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
    commandDetails.push_back("false");
    commandDetails.push_back("false");

    iCommand* luaCommand = g_currentScene->m_commandFactory->CreateCommandObject(lua_tostring(L, 2), commandDetails);

    if (g_currentScene->m_commandManager->m_IsCreatingCommandGroup)
    {
        g_currentScene->m_commandManager->m_CommandGroupBeingCreated->addSerial(luaCommand);
    }
    else
    {
        cCommandGroup* serialCommand = new cCommandGroup();
        serialCommand->addSerial(luaCommand);
        g_currentScene->m_commandManager->AddCommandGroup(serialCommand);
    }

    return 1;
}

int LuaCreateCommandGroup(lua_State* L)
{
    g_currentScene->m_commandManager->m_IsCreatingCommandGroup = true;
    g_currentScene->m_commandManager->m_CommandGroupBeingCreated = new cCommandGroup();
    return 1;
}

int LuaCompleteCommandGroup(lua_State* L)
{
    g_currentScene->m_commandManager->m_IsCreatingCommandGroup = false;
    g_currentScene->m_commandManager->AddCommandGroup(g_currentScene->m_commandManager->m_CommandGroupBeingCreated);
    return 1;
}

int LuaAddParallelCommand(lua_State* L)
{
    std::string friendlyName = lua_tostring(L, 1);

    cGameObject* pGO = cLuaManager::FindObjectByFriendlyName(friendlyName);

    if (pGO == nullptr)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    std::vector<std::string> commandDetails;
    commandDetails.push_back(friendlyName);
    commandDetails.push_back(std::to_string(lua_tonumber(L, 3)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));

    commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 7)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 8)));

    commandDetails.push_back(std::to_string(lua_tonumber(L, 9)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 10)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 11)));

    commandDetails.push_back(std::to_string(lua_tonumber(L, 12)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 13)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 14)));

    commandDetails.push_back(std::to_string(lua_tonumber(L, 15)));

    iCommand* luaCommand = g_currentScene->m_commandFactory->CreateCommandObject(lua_tostring(L, 2), commandDetails);

    if (g_currentScene->m_commandManager->m_IsCreatingCommandGroup)
    {
        g_currentScene->m_commandManager->m_CommandGroupBeingCreated->addParallel(luaCommand);
    }
    else
    {
        cCommandGroup* serialCommand = new cCommandGroup();
        serialCommand->addParallel(luaCommand);
        g_currentScene->m_commandManager->AddCommandGroup(serialCommand);
    }

    return 1;
}

int LuaAddLocationTriggerCommand(lua_State* L)
{
    std::string triggeredFriendlyName = lua_tostring(L, 1);
    std::string commandedFriendlyName = lua_tostring(L, 2);

    cGameObject* triggeredObject = cLuaManager::FindObjectByFriendlyName(triggeredFriendlyName);
    cGameObject* commandedObject = cLuaManager::FindObjectByFriendlyName(commandedFriendlyName);

    if (triggeredObject == nullptr || commandedObject == nullptr)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    std::vector<std::string> commandDetails;
    commandDetails.push_back(triggeredFriendlyName);
    commandDetails.push_back(commandedFriendlyName);

    commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 7)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 8)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 9)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 10)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 11)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 12)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 13)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 14)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 15)));

    iCommand* luaCommand = g_currentScene->m_commandFactory->CreateTriggerCommandObject(lua_tostring(L, 3), commandDetails);

    if (g_currentScene->m_commandManager->m_IsCreatingCommandGroup)
    {
        g_currentScene->m_commandManager->m_CommandGroupBeingCreated->addParallel(luaCommand);
    }
    else
    {
        cCommandGroup* commandGroup = new cCommandGroup();
        commandGroup->addSerial(luaCommand);
        glm::vec3 triggerPosition = glm::vec3(
            atof(commandDetails[2].c_str()),
            atof(commandDetails[3].c_str()),
            atof(commandDetails[4].c_str()));

        g_currentScene->m_commandManager->AddTriggerCommandGroups(
            triggerPosition, commandGroup, triggeredFriendlyName, triggeredObject->GetObjectPhysics(), (float)atof(commandDetails[5].c_str()));
    }

    return 1;
}

int LuaAddLoopingAnimation(lua_State* L)
{
    std::string loopingObjectFriendlyName = lua_tostring(L, 1);

    cGameObject* loopingObject = cLuaManager::FindObjectByFriendlyName(loopingObjectFriendlyName);

    if (loopingObject == nullptr)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    std::vector<std::string> commandDetails;
    commandDetails.push_back(loopingObjectFriendlyName);

    commandDetails.push_back(std::to_string(lua_tonumber(L, 3)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 7)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 8)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 9)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 10)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 11)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 12)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 13)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 14)));
    commandDetails.push_back(std::to_string(lua_tonumber(L, 15)));

    iCommand* luaCommand = g_currentScene->m_commandFactory->CreateLoopingCommandObject(lua_tostring(L, 2), commandDetails);
    cCommandGroup* commandGroup = new cCommandGroup();
    commandGroup->addSerial(luaCommand);
    g_currentScene->m_commandManager->AddLoopingCommandGroup(commandGroup);

    return 1;
}