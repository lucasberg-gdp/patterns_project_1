#pragma once
#include <vector>
#include "cScene.h"
#include "cSquad.h"
#include "cMediaPlayer.h"

class Game : public iSystem
{
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
	double skirmishCooldown = 2.5;

	double gameStartDelayTimer = 0.0;
	double gameStartDelay = 1.0;

	double audioTransitionTime = 1.0;
	double audioTransitionTimeElapsed = 0.0;
	void TransitionToBattleSong();
	void TransitionToMenuSong();
	
	bool isSongTransitioningToBattle = false;
	bool isSongTransitioningToMenu = false;
	
	double soundTransitionSpeed = 0.1f;
	void UpdateSounds(double deltaTime);
public:
	bool isInMenu = true;

	cMediaPlayer* mediaPlayer = nullptr;

	Game();
	void SendNextSquad();
	void CreateStageSquads();

	void StartSkirmishing();
	void UpdateSkirmish(double deltaTime);

	void SkirmishWithAllEnemies();

	void StartGame();
	void PauseGame();
	void RestartGame();
	void RetryLevel();
	void RevivePlayer();

	void PressEnter();

	void ResetGameTimers();

	bool IsGameOver();
	bool IsStageComplete();

	std::vector<cMediaPlayer* > soundsPlaying;

	std::vector<cSquad> squads;
	double timerForNextSquad = -3.0;

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