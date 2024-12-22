#pragma once

#include "cMesh.h"
#include "c2DNavigation.h"

class cEnemyManager
{
public:
	enum movementType {
		Idle,
		Line,
		Sine,
		Cosine,
		CurveLeftRight,
		Skirmishing
	};

private:
	bool m_IsMoving = false;
	bool m_FaceMovementDirection = true;
	bool m_IsSkirmishing = false;

	double m_MovementSpeed = 1.0;

	float m_XStartPosition = 0.0f;
	float m_YAdvancedPosition = 0.0f;

	glm::vec3 m_InitialPosition = glm::vec3(0.0f);
	glm::vec3 m_FinalPosition = glm::vec3(0.0f);
	glm::vec3 m_CurrentPosition = glm::vec3(0.0f);
	glm::vec3 m_LastPosition = glm::vec3(0.0f);

	glm::vec3 m_GridPosition = glm::vec3(0.0f);
	double m_EnemySpeed = 0.0;

	movementType m_MovementType = movementType::Idle;

	cMesh* m_EnemyMesh = nullptr;

	c2DNavigation m_2DNavigation;
	std::vector<glm::vec3> m_BeePositions;

	std::vector<glm::vec3> GetMovementPositions(unsigned int numberOfPositions, glm::vec3 intialPosition, glm::vec3 finalPosition);

	int m_CurrentPositionIndex = 0;
	bool m_FinishedIntroNavigation = false;
	void NavigateToNextPosition(double deltaTime);
	void NavigateToGrid(double deltaTime);
	void NavigateForSkirmish(double deltaTime);

	bool m_IsMovingToGrid = false;
	bool m_IsInGrid = false;

public:
	std::string m_EnemyType;

	cEnemyManager() {}
	cEnemyManager(std::string enemyType);
	cEnemyManager(std::string enemyType, cMesh* mesh, glm::vec3 initialPosition, glm::vec3 finalPosition, double speed);

	void SetLeftToRightIntroMovement();
	void SetRightToLeftIntroMovement();

	void SetLeftToRightFullCircleIntroMovement();
	void SetRightToLeftFullCircleIntroMovement();

	void StartMoving(glm::vec3 position);
	void StartMovingToGrid();
	void StartSkirmishing();
	void FinishSkirmish();

	void SelectMovementType(movementType movementType);

	void Update(double deltaTime);
	void UpdatePosition(double deltaTime);

	glm::vec3 GetPositionOnLine(double deltaTime);
	glm::vec3 GetPositionOnSine(double deltaTime);

	void FaceMovementDirection();
	void ArrivedAtDestination();

	void SkipIntro();
};

