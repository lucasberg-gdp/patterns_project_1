#include "cMeshAnimation.h"


void cMeshAnimation::AddAnimation(std::string meshName)
{
	m_MeshAnimations.push_back(meshName);
}

void cMeshAnimation::SetAnimationTime(double time)
{
	m_AnimationTime = time;
}

void cMeshAnimation::StartAnimation()
{
	m_IsAnimating = true;
	animationTimer = m_AnimationTime;
	m_CurrentMeshNameIndex = 0;
	m_CurrentMeshName = m_MeshAnimations[m_CurrentMeshNameIndex];
}

std::string cMeshAnimation::MoveToNextAnimation()
{
	if (m_IsLooping)
	{
		// TODO: Check this. Very mysterious
		if ((m_CurrentMeshNameIndex + 1) == m_MeshAnimations.size())
		{

		}
	}
	else
	{

	}

	return m_CurrentMeshName;
}

void cMeshAnimation::StopAnimation()
{
	m_IsAnimating = false;
}

void cMeshAnimation::PauseAnimation()
{
	m_IsAnimating = false;
}

bool cMeshAnimation::IsAnimationOver()
{
	return m_IsAnimationOver;
}

void cMeshAnimation::Update(double deltaTime)
{
	if (!m_IsAnimating)
	{
		return;
	}

	animationTimer -= deltaTime;

	if (animationTimer <= 0.0) 
	{
		if (m_CurrentMeshNameIndex + 1 >= m_MeshAnimations.size())
		{
			if (m_IsLooping)
			{
				m_CurrentMeshNameIndex = 0;
			}
			else
			{
				m_IsAnimationOver = true;
				m_IsAnimating = false;
			}
		}
		else
		{
			++m_CurrentMeshNameIndex;
		}

		m_CurrentMeshName = m_MeshAnimations[m_CurrentMeshNameIndex];

		animationTimer = m_AnimationTime;
	}
}

std::string cMeshAnimation::GetCurrentMeshName()
{
	if (m_CurrentMeshNameIndex + 1 < m_MeshAnimations.size())
	{
		m_CurrentMeshName = m_MeshAnimations[m_CurrentMeshNameIndex];
		return m_CurrentMeshName;
	}

	return "";
}

