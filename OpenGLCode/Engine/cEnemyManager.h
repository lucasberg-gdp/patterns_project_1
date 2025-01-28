#pragma once

#include "cMesh.h"
#include "c2DNavigation.h"
#include "iEnemy.h"

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
	bool m_IsFacingMovementDirection = true;
	bool m_IsSkirmishing = false;
	bool m_IsMakingSkirmishRound = false;

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

	iEnemy* m_Enemy = nullptr;

	c2DNavigation m_2DNavigation;
	std::vector<glm::vec3> m_EnemyPositions;

	std::vector<glm::vec3> m_IntroBezierControlPoints;
	std::vector<glm::vec3> m_RoundBezierControlPoints;

	std::vector<glm::vec3> GetMovementPositions(unsigned int numberOfPositions, glm::vec3 intialPosition, glm::vec3 finalPosition);

	int m_CurrentPositionIndex = 0;
	bool m_FinishedIntroNavigation = false;
	void NavigateToNextPosition(double deltaTime);
	void NavigateToNextPositionOnBezierCurve(double deltaTime);
	void MakeRoundOnBezierCurve(double deltaTime);
	void NavigateToGrid(double deltaTime);
	void NavigateForSkirmish(double deltaTime);

	bool m_IsMovingToGrid = false;
	bool m_IsInGrid = false;

	bool m_IsMakingRound = false;

public:
	double m_ElapsedTime = 0.0;
	double m_TimeToMove = 3.0;
	double m_TimeToMakeRound = 1.5;

	std::string m_EnemyType;

	cEnemyManager() {}
	cEnemyManager(iEnemy* enemy);
	cEnemyManager(std::string enemyType);
	cEnemyManager(iEnemy* enemy, std::string enemyType, cMesh* mesh, glm::vec3 initialPosition, glm::vec3 finalPosition, double speed);

	void SetBezierIntroMovement();
	void SetInvertedBezierIntroMovement();

	void SetBezierIntroLeftRightMovement();
	void SetBezierInvertedIntroLeftRightMovement();

	void StartMoving(glm::vec3 position);
	void StartMovingToGrid();
	void StartSkirmishing();
	void FinishSkirmish();

	void MakeRoundForSkirmish(double deltaTime);

	void SelectMovementType(movementType movementType);

	void Update(double deltaTime);
	void UpdatePosition(double deltaTime);

	glm::vec3 GetPositionOnLine(double deltaTime);
	glm::vec3 GetPositionOnSine(double deltaTime);

	void FaceMovementDirection();
	void ArrivedAtDestination();

	void SkipIntro();
};

