#pragma once

#include "cMesh.h"

class cStars
{
	bool m_IsChangingSpeed = false;
	bool m_IsIncreasingSpeed = false;
	bool m_IsDecreasingSpeed = false;

	float m_MinX = -11000.0f;
	float m_MaxX = 11000.0f;

	float m_MinY = -4500.0f;
	float m_MaxY = 4500.0f;

	float m_ZPosition = -10000.f;

	float m_DescendSpeed = -200.0f;

	float m_SlowDescendSpeed = -100.0f;
	float m_FastDescendSpeed = -1500.0f;

	double m_TimeToChangeSpeed = 1.0f;
	double m_TimeElapsedToChangeSpeed = 0.0f;

	unsigned int m_StarsVolume = 100;

public:
	std::vector<cMesh*> stars;

	cStars(unsigned int starsVolume, float descendSpeed);

	cStars(unsigned int starsVolume, float slowSpeed, float fastSpeed, double timeToChangeSpeed);

	void Update(double deltaTime);

	cMesh* CreateStar();

	float GetTopOfLevel();

	void IncreaseSpeed();
	void DecreaseSpeed();
};

