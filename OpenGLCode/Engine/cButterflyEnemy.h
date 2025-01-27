#pragma once
#include "cEnemy.h"
#include "cEnemyManager.h"
#include "cEnemyBulletFactory.h"

class cButterflyEnemy : public cEnemy
{
public:
	cButterflyEnemy();

	void ShootBullet();

	// Inherited via iEnemy
	void DamageEnemy(int damage) override;
	void DestroyEnemy() override;
	void UpdatePosition(double deltaTime) override;

	std::string GetEnemyType() override;

	void Update(double deltaTime) override;
};