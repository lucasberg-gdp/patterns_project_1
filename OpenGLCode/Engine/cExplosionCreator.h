#pragma once

#include <glm/vec3.hpp>

#include <vector>
#include "cExplosion.h"
#include "cMeshAnimation.h"

class cExplosionCreator
{
	double m_ExplosionSpeed = 0.0;

	std::vector<std::string> m_MeshAnimations;

	std::vector<cExplosion*> m_CurrentExplosions;

public:
	cExplosionCreator();
	~cExplosionCreator();

	cExplosionCreator(std::vector < std::string > meshAnimations);

	void SetExplosionSpeed(double animationSpeed);

	void CreateExplosion(glm::vec3 explosionPosition);
	void CreateExplosion(glm::vec3 explosionPosition, double animationTime);
	void Update(double deltaTime);

	void RemoveExplosion(cExplosion* explosion);
};