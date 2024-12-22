#pragma once

#include "iBulletFactory.h"

#include <string>

class cEnemyBulletFactory: public iBulletFactory
{
	float m_EnemyBulletSpeed = 1000.0f;
	std::string m_EnemyBulletMesh = "enemyBullet.ply";
public:
	void CreateBullet(float x, float y, float z) override;
};
