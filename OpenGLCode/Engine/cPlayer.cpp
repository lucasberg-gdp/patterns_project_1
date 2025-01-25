#include "cPlayer.h"
#include "cCollisionMediator.h"
#include "cScene.h"
#include "game.h"
#include "cStars.h"

extern cCollisionMediator* g_CollisionMediator;

extern cScene* g_currentScene;
extern Game game;

extern cStars* stars;

cPlayer::cPlayer()
{
	cMesh* playerShipTester = new cMesh();
	playerShipTester->meshName = "playerShip.ply";
	playerShipTester->isUsingVertexColors = true;
	playerShipTester->bUseDebugColours = false;
	playerShipTester->bDoNotLight = true;
	playerShipTester->drawPosition = glm::vec3(0.0f);
	playerShipTester->drawPosition.y = -1300.0f;
	m_PlayerMesh = playerShipTester;

	m_PlayerMovement = cShipMovement(m_PlayerMesh);
	m_PlayerMovement.SetSpeed(m_PlayerSpeed);

	m_PlayerMovement.StartMoving();
}

cPlayer::cPlayer(int playerLives) :
	cPlayer()
{
	m_NumberOfExtraLives = playerLives;

	glm::vec3 firstExtraLifePosition = m_PlayerMesh->drawPosition;

	firstExtraLifePosition.y -= 100.0f;
	firstExtraLifePosition.x += 1500.0f;

	for (int i = 0; i < playerLives - 1; i++)
	{
		cMesh* playerShip = new cMesh();
		playerShip->meshName = "playerShip.ply";
		playerShip->isUsingVertexColors = true;
		playerShip->bUseDebugColours = false;
		playerShip->bDoNotLight = true;
		playerShip->drawPosition = firstExtraLifePosition;
		playerShip->drawPosition.x += (i * 200.0f);

		m_ExtraLives.push_back(playerShip);

		g_currentScene->AddMesh(playerShip);
	}
}

cPlayer::~cPlayer()
{

}

void cPlayer::SetPlayerMesh(cMesh* playerMesh)
{
	m_PlayerMesh = playerMesh;
}

cMesh* cPlayer::GetPlayerMesh()
{
	return m_PlayerMesh;
}

void cPlayer::SetPlayerSpeed(double speed)
{
	m_PlayerSpeed = speed;
}

void cPlayer::SetIsPressingLeft(bool isPressingLeft)
{
	m_IsPressingLeft = isPressingLeft;
}

void cPlayer::SetIsPressingRight(bool isPressingRight)
{
	m_IsPressingRight = isPressingRight;
}

void cPlayer::SetPlayerBulletCooldown(double bulletCooldown)
{
	m_BulletCooldown = bulletCooldown;
}

void cPlayer::Update(double deltaTime)
{
	UpdatePosition(deltaTime);
	UpdateBulletCooldown(deltaTime);

	if (m_IsReviving)
	{
		m_RevivingTimer += deltaTime;

		if (m_RevivingTimer > m_RevivingCooldown)
		{
			m_RevivingTimer = 0.0;

			m_IsReviving = false;
			bool hasLifeLeft = false;

			for (int i = (int)m_ExtraLives.size() - 1; i >= 0; i--)
			{
				if (m_ExtraLives[i]->bIsVisible)
				{
					m_ExtraLives[i]->bIsVisible = false;
					hasLifeLeft = true;
					break;
				}
			}

			if (hasLifeLeft)
			{
				m_PlayerMesh->bIsVisible = true;
				m_IsPlayerAlive = true;
			}
			else
			{
				GameOver();
			}
		}
	}
}

glm::vec3 cPlayer::GetPlayerPosition()
{
	return m_PlayerMesh->drawPosition;
}

void cPlayer::UpdatePosition(double deltaTime)
{
	if (!m_IsPlayerAlive)
	{
		return;
	}

	glm::vec3 movementDirection = glm::vec3(0.0f);

	if (m_IsPressingLeft && !m_IsPressingRight)
	{
		movementDirection.x = -1.0f;
	}

	if (m_IsPressingRight && !m_IsPressingLeft)
	{
		movementDirection.x = 1.0f;

	}

	m_PlayerMovement.SetDirection(movementDirection);
	m_PlayerMovement.Update(deltaTime);
}

void cPlayer::UpdateBulletCooldown(double deltaTime)
{
	m_BulletCooldownTimer -= deltaTime;

	if (m_BulletCooldownTimer <= 0.0f)
	{
		m_canShootBullet = true;
	}
	else
	{
		m_canShootBullet = false;
	}
}

void cPlayer::ShootBullet()
{
	if (m_canShootBullet && !game.isInMenu)
	{
		bulletFactory.CreateBullet(m_PlayerMesh->drawPosition.x, m_PlayerMesh->drawPosition.y, m_PlayerMesh->drawPosition.z);
		m_BulletCooldownTimer = m_BulletCooldown;
	}
}

void cPlayer::HitPlayer(int damage)
{
	m_PlayerHealth -= damage;

	if (m_PlayerHealth <= 0)
	{
		DestroyPlayer();
	}
}

bool cPlayer::IsDead()
{
	return !m_IsPlayerAlive;
}

void cPlayer::DestroyPlayer()
{
	m_IsPlayerAlive = false;
	g_CollisionMediator->RemovePlayer();

	RevivePlayer();
}

void cPlayer::InstantlyRevivePlayer()
{
	m_PlayerMesh->bIsVisible = true;
	m_IsPlayerAlive = true;

	for (int i = (int)m_ExtraLives.size() - 1; i >= 0; i--)
	{
		m_ExtraLives[i]->bIsVisible = true;
	}
}

void cPlayer::RevivePlayer()
{
	m_IsReviving = true;
}

void cPlayer::GameOver()
{
	m_IsPlayerAlive = false;
	g_currentScene->worldText->SetText("game over");
	stars->DecreaseSpeed();
}

void cPlayer::DisablePlayer()
{
	m_PlayerMesh->bIsVisible = false;
}

void cPlayer::EnablePlayer()
{
	m_PlayerMesh->bIsVisible = true;
}
