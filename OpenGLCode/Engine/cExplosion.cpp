#include "cExplosion.h"

cExplosion::cExplosion(glm::vec3 position)
{
	m_ExplosionMesh = new cMesh("explosion");
	m_ExplosionMesh->bDoNotLight = true;
	m_ExplosionMesh->isUsingVertexColors = true;
	m_ExplosionMesh->drawPosition = position;
}

cExplosion::~cExplosion()
{

}

void cExplosion::AddAnimationMesh(std::string meshName)
{
	m_ExplosionAnimation.AddAnimation(meshName);
}

void cExplosion::SetAnimationTimer(double animationTime)
{
	m_ExplosionAnimation.SetAnimationTime(animationTime);
}

void cExplosion::StartExplosion()
{
	m_IsExploding = true;
	m_ExplosionMesh->meshName = m_ExplosionAnimation.GetCurrentMeshName();
	m_ExplosionAnimation.StartAnimation();
}

void cExplosion::Update(double deltaTime)
{
	if (!m_IsExploding)
	{
		return;
	}

	m_ExplosionAnimation.Update(deltaTime);

	if (m_ExplosionMesh)
	{
		m_ExplosionMesh->meshName = m_ExplosionAnimation.GetCurrentMeshName();
	}
}

cMesh* cExplosion::GetMesh()
{
	return m_ExplosionMesh;
}

bool cExplosion::IsExplosionOver()
{
	m_IsExploding = !m_ExplosionAnimation.IsAnimationOver();
	return m_ExplosionAnimation.IsAnimationOver();
}
