#pragma once

class iBulletFactory
{
public:
	virtual ~iBulletFactory() = default;
	virtual void CreateBullet(float xPosition, float yPosition, float zPosition) = 0;
};