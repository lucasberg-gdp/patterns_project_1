#pragma once

#include "iBullet.h"

class cPlayerBullet: public iBullet
{
	bool m_IsBulletUsed = false;

	float m_MaxY = 1500.0f;

	cMesh* m_BulletMesh;
	glm::vec3 m_BulletSpeed;

public:
	cPlayerBullet();
	//cPlayerBullet(glm::vec3 initialPosition, glm::vec3 velocity, std::string playerBulletMesh);

	cPlayerBullet(glm::vec3 initialPosition, float speed, std::string playerBulletMesh);

	~cPlayerBullet();

	// Inherited via iBullet
	void UpdatePosition(double deltaTime) override;
	cMesh* GetBulletMesh() override;
	bool IsBulletUsed() override;
	void BulletHit() override;
	void SetBulletSpeed(glm::vec3 bulletSpeed) override;
	std::string GetBulletType() override;
};

