#pragma once

#include "cMesh.h"

class cStars
{
	float m_MinX = -11000.0f;
	float m_MaxX = 11000.0f;

	float m_MinY = -4500.0f;
	float m_MaxY = 4500.0f;

	float m_ZPosition = -10000.f;

	float m_DescendSpeed = -100.0f;

	unsigned int m_StarsVolume = 100;

public:
	std::vector<cMesh*> stars;

	cStars(unsigned int starsVolume, float descendSpeed);

	void Update(double deltaTime);

	cMesh* CreateStar();

	float GetTopOfLevel();
};

