#pragma once

#include "cPlayerBulletFactory.h"
#include "cShipMovement.h"
#include "cMediaPlayer.h"

class cPlayer
{
	bool m_IsPlayerAlive = true;
	bool m_IsReviving = false;

	double m_RevivingTimer = 0.0;
	double m_RevivingCooldown = 0.5f;

	bool m_IsPressingLeft = false;
	bool m_IsPressingRight = false;
	bool m_IsShootingBullet = false;

	int m_PlayerHealth = 1;
	double m_PlayerSpeed = 1000.0;

	cMesh* m_PlayerMesh = nullptr;

	int m_NumberOfExtraLives = 1;
	std::vector<cMesh*> m_ExtraLives;

	cPlayerBulletFactory bulletFactory;

	bool m_canShootBullet = true;
	double m_BulletCooldown = 0.9f;
	double m_BulletCooldownTimer = 0.0f;

	cShipMovement m_PlayerMovement = nullptr;

	cMediaPlayer* m_PlayerLaserSound = nullptr;

	void UpdatePosition(double deltaTime);
	void UpdateBulletCooldown(double deltaTime);
public:
	cPlayer();
	cPlayer(int playerLives);

	~cPlayer();

	void SetPlayerMesh(cMesh* playerMesh);

	cMesh* GetPlayerMesh();

	void SetPlayerSpeed(double speed);

	void Update(double deltaTime);

	glm::vec3 GetPlayerPosition();

	void SetIsPressingLeft(bool isPressingLeft);
	void SetIsPressingRight(bool isPressingRight);
	void SetPlayerBulletCooldown(double bulletCooldown);

	void ShootBullet();

	void HitPlayer(int damage);
	bool IsDead();
	void DestroyPlayer();

	void InstantlyRevivePlayer();
	void RevivePlayer();
	void GameOver();

	void DisablePlayer();
	void EnablePlayer();
};