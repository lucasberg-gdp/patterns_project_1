#pragma once

#include "cEnemyManager.h"
#include "cEnemyBulletFactory.h"
#include "cEnemy.h"

class cMothEnemy :  public cEnemy
{
	std::string m_GreenMoth = "greenMothEnemy.ply";
	std::string m_BlueMoth = "blueMothEnemy.ply";
public:
	cMothEnemy();
	~cMothEnemy();

	void ShootBullet();

	// Inherited via iEnemy
	void DamageEnemy(int damage) override;
	void DestroyEnemy() override;
	void UpdatePosition(double deltaTime) override;

	std::string GetEnemyType() override;

	void Update(double deltaTime) override;
};