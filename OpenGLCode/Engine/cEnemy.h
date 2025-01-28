#pragma once

#include "cMesh.h"
#include "cEnemyManager.h"
#include "cEnemyBulletFactory.h"
#include "iEnemy.h"

class cEnemy: public iEnemy
{
protected:
	bool m_IsDead = false;
	bool m_IsInGrid = false;
	bool m_IsAttacking = false;
	bool m_IsSkirmishing = false;

	double m_BulletTimer = 0.0;
	double m_BulletCooldown = 0.3;
	bool m_CanShootBullet = false;

	int m_Health = 0;
	int m_EnemyPoints = 100;

	cMesh* m_EnemyMesh = nullptr;

	cEnemyManager::movementType m_MovementType = cEnemyManager::movementType::Sine;

	cEnemyBulletFactory bulletFactory;

	cEnemyManager m_EnemyManager;

	double timer = 0.0;
public:

	void SetIntroType(std::string introType) override;
	void Attack() override;
	void Skirmish() override;

	bool IsDead() override;
	int GetPoints() override;

	void SetHealth(int health) override;

	cMesh* GetEnemyMesh() override;

	bool IsInGrid() override;
	void SetIsInGrid(bool isInGrid) override;
};