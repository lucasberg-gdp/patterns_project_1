#pragma once

#include <glm/vec3.hpp>

#include <vector>
#include <string>
#include "cMeshAnimation.h"

class cExplosion
{
	bool m_IsExploding = false;
	cMeshAnimation m_ExplosionAnimation;

	cMesh* m_ExplosionMesh = nullptr;
public:
	cExplosion(glm::vec3 position);
	~cExplosion();

	void AddAnimationMesh(std::string meshName);
	void SetAnimationTimer(double animationTime);

	void StartExplosion();

	void Update(double deltaTime);

	cMesh* GetMesh();

	bool IsExplosionOver();
};

