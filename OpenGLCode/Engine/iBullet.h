#pragma once
#include "cMesh.h"


class iBullet {

public:
	virtual ~iBullet() = default;

	virtual void SetBulletSpeed(glm::vec3 bulletSpeed) = 0;

	virtual void UpdatePosition(double deltaTime) = 0;
	virtual cMesh* GetBulletMesh() = 0;

	virtual bool IsBulletUsed() = 0;
	virtual void BulletHit() = 0;

	virtual std::string GetBulletType() = 0;
};