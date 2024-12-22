#pragma once
#include "cMesh.h"

class cShipMovement
{
	bool m_IsMoving = false;

	bool m_IsFacingDirection = false;
	double m_ShipSpeed = 0.0;

	glm::vec3 m_CurrentDirection = glm::vec3(0.0f);

	cMesh* m_ShipMesh = nullptr;

	void MoveShip(double deltaTime);

public:
	cShipMovement(cMesh* ship);

	void Update(double deltaTime);
	
	void SetDirection(glm::vec3 direction);
	void SetSpeed(double shipSpeed);
	void StartMoving();
	void StopMoving();

	void SetIsFacingDirection(bool isFacingDirection);
};

