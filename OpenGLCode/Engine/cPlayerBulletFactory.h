#pragma once

#include "iBulletFactory.h"

#include <string>

class cPlayerBulletFactory: public iBulletFactory
{
	float m_PlayerBulletSpeed = 2500.0f;
	std::string m_PlayerBulletMesh = "playerBullet.ply";
public:
	// Inherited via cBulletFactory
	void CreateBullet(float x, float y , float z) override;
};