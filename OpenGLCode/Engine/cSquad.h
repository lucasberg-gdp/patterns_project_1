#pragma once

#include <vector>
#include "iEnemy.h"

struct cSquad
{
	bool m_IsSquadRunning = false;
	bool m_IsSquadDone = false;

	int m_NumberOfBees = 0;
	int m_NumberOfButterflies = 0;
	int m_NumberOfMoths = 0;

	int m_ShipsPerCoolDown = 1;

	double timer = 0.0;
	double timeForNextShip = 0.25;

	int m_CurrentShipIndex = 0;
	std::vector<iEnemy*> m_EnemiesInSquad;

	cSquad(){}

	cSquad(int numberOfBees, int numberOfButterflies, int numberOfMoths, int shipsPerCoolDown);

	cSquad(int numberOfBees, int numberOfButterflies, int numberOfMoths, int shipsPerCoolDown, bool alternateEnemies);

	cSquad(int numberOfBeesAndButterflies, int shipsPerCoolDown, bool alternateEnemies);

	void CreateBeeAndButterflySquad(int numberOfBeesAndButterflies, int shipsPerCoolDown, bool alternateEnemies);
	void CreateButterflyAndMothsSquad(int numberOfButterfliesAndMoths, int shipsPerCoolDown, bool alternateEnemies);

	void Update(double deltaTime);

	void SetBeesMovementType(std::string introType);
	void SetButterfliesMovementType(std::string introType);
	void SetMothsMovementType(std::string introType);

	void SortByShipType();
};

