#pragma once

#include <vector>
#include <string>

#include "cMesh.h"

class cMeshAnimation
{
	bool m_IsAnimating = false;
	bool m_IsAnimationOver = false;

	bool m_IsLooping = false;
	std::string m_CurrentMeshName;

	double m_AnimationTime = 0.0;
public:
	int m_CurrentMeshNameIndex = 0;
	std::vector<std::string> m_MeshAnimations;

	double animationTimer = 0.0;

	void AddAnimation(std::string meshName);
	void SetAnimationTime(double time);
	void StartAnimation();
	std::string MoveToNextAnimation();

	void StopAnimation();
	void PauseAnimation();

	bool IsAnimationOver();

	void Update(double deltaTime);
	
	std::string GetCurrentMeshName();

};

