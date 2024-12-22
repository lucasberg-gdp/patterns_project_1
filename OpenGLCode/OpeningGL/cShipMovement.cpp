#include "cShipMovement.h"

cShipMovement::cShipMovement(cMesh* ship)
{
	m_ShipMesh = ship;
}

void cShipMovement::Update(double deltaTime)
{
	if (m_IsMoving)
	{
		MoveShip(deltaTime);
	}
}

void cShipMovement::MoveShip(double deltaTime)
{
	m_ShipMesh->drawPosition.x += m_CurrentDirection.x * (float)(deltaTime * m_ShipSpeed);
	m_ShipMesh->drawPosition.y += m_CurrentDirection.y * (float)(deltaTime * m_ShipSpeed);
	m_ShipMesh->drawPosition.z += m_CurrentDirection.z * (float)(deltaTime * m_ShipSpeed);
}

void cShipMovement::SetDirection(glm::vec3 direction)
{
	m_CurrentDirection = direction;
}

void cShipMovement::SetSpeed(double shipSpeed)
{
	m_ShipSpeed = shipSpeed;
}

void cShipMovement::StartMoving()
{
	m_IsMoving = true;
}

void cShipMovement::StopMoving()
{
	m_IsMoving = false;
}

void cShipMovement::SetIsFacingDirection(bool isFacingDirection)
{
	m_IsFacingDirection = isFacingDirection;
}
