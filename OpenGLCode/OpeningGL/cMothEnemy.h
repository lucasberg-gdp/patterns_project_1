#pragma once

#include "iEnemy.h"
#include "cEnemyManager.h"
#include "cEnemyBulletFactory.h"

class cMothEnemy : public iEnemy
{
	bool m_IsDead = false;
	bool m_IsInGrid = false;
	bool m_IsAttacking = false;
	bool m_IsSkirmishing = false;

	double m_BulletTimer = 0.0;
	double m_BulletCooldown = 0.3;
	bool m_CanShootBullet = false;

	int m_Health = 0;
	int m_EnemyPoints = 150;

	cMesh* m_EnemyMesh = nullptr;
	cEnemyManager::movementType m_MovementType;

	std::string m_GreenMoth = "greenMothEnemy.ply";
	std::string m_BlueMoth = "blueMothEnemy.ply";

	cEnemyManager m_EnemyManager;
	cEnemyBulletFactory bulletFactory;

	double timer = 0.0;
public:
	cMothEnemy();
	~cMothEnemy();

	void ShootBullet();

	// Inherited via iEnemy
	void SetHealth(int health) override;
	void DamageEnemy(int damage) override;
	void DestroyEnemy() override;
	void UpdatePosition(double deltaTime) override;

	std::string GetEnemyType() override;
	cMesh* GetEnemyMesh() override;

	void Attack() override;

	void Update(double deltaTime) override;

	// Inherited via iEnemy
	void SetIntroType(std::string introType) override;

	// Inherited via iEnemy
	void Skirmish() override;

	// Inherited via iEnemy
	bool IsDead() override;

	// Inherited via iEnemy
	int GetPoints() override;
};