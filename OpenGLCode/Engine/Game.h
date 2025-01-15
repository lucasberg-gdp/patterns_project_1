#pragma once
#include <vector>
#include "cScene.h"
#include "cSquad.h"

class Game : public iSystem
{
	//double timeForNextShip = 0.1;
	bool isGamePaused = false;
	bool isGameOver = false;
	bool isStageComplete = false;

	double squadTimer = 0.0;
	int currentShipIndex = 0;
	bool isFirstSquadIsDone = false;
	bool isFirstSquadRunning = false;

	bool isSecondSquadIsDone = false;
	bool isSecondSquadRunning = false;

	bool isSkirmishing = false;
	double skirmishTimer = 0.0;
	double skirmishCooldown = 5.5;

	double gameStartDelayTimer = 0.0;
	double gameStartDelay = 1.0;

public:
	bool isInMenu = true;

	Game();
	void SendNextSquad();

	void SendFirstSquad();
	void SendSecondSquad();
	void CreateStageSquads();

	void StartSkirmishing();
	void UpdateSkirmish(double deltaTime);

	void SkirmishWithAllEnemies();

	void StartGame();
	void PauseGame();
	void RestartGame();
	void RetryLevel();
	void RevivePlayer();

	void ResetGameTimers();

	bool IsGameOver();
	bool IsStageComplete();

	std::vector<cSquad> squads;
	double timerForNextSquad = -5.0;

	int currentSquadIndex = 0;
	void UpdateCurrentSquad(double deltaTime);

	std::vector<cScene> gameScenes;

	void Awake(GLFWwindow& window) override;
	void Start(GLFWwindow& window) override;
	void EarlyUpdate(GLFWwindow& window, double deltaTime) override;
	void Update(GLFWwindow& window, double deltaTime) override;
	void LateUpdate(GLFWwindow& window, double deltaTime) override;
	void End(GLFWwindow& window) override;

	// Inherited via iSystem
	std::string GetSystemType() override;
};