#pragma once

#include "cEnemy.h"
#include "cMesh.h"
#include "cEnemyBulletFactory.h"
#include "cEnemyManager.h"

class cBeeEnemy : public cEnemy
{
public:
	cBeeEnemy();
	~cBeeEnemy();

	void ShootBullet();

	// Inherited via iEnemy
	void SetHealth(int health) override;
	void DamageEnemy(int damage) override;
	void DestroyEnemy() override;
	void UpdatePosition(double deltaTime) override;

	std::string GetEnemyType() override;

	// Inherited via iEnemy
	void Update(double deltaTime) override;

	// Inherited via iEnemy
	bool IsDead() override;

	// Inherited via iEnemy
	int GetPoints() override;
};