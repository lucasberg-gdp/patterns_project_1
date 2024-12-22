#include "cExplosionCreator.h"
#include "cScene.h"

extern cScene* g_currentScene;

cExplosionCreator::cExplosionCreator()
{

}

cExplosionCreator::~cExplosionCreator()
{

}

cExplosionCreator::cExplosionCreator(std::vector<std::string> meshAnimations)
{
	for (std::string meshAnimation : meshAnimations)
	{
		m_MeshAnimations.push_back(meshAnimation);
	}
}

void cExplosionCreator::SetExplosionSpeed(double animationSpeed)
{
	m_ExplosionSpeed = animationSpeed;
}

void cExplosionCreator::CreateExplosion(glm::vec3 explosionPosition)
{
	cExplosion* newExplosion = new cExplosion(explosionPosition);

	for (std::string meshAnimation : m_MeshAnimations)
	{
		newExplosion->AddAnimationMesh(meshAnimation);
	}

	newExplosion->SetAnimationTimer(m_ExplosionSpeed);
	newExplosion->StartExplosion();

	m_CurrentExplosions.push_back(newExplosion);

	cMesh* explosionMesh = newExplosion->GetMesh();

	g_currentScene->AddMesh(explosionMesh);
}

void cExplosionCreator::CreateExplosion(glm::vec3 explosionPosition, double animationTime)
{
	cExplosion* newExplosion = new cExplosion(explosionPosition);

	for (std::string meshAnimation : m_MeshAnimations)
	{
		newExplosion->AddAnimationMesh(meshAnimation);
	}

	newExplosion->SetAnimationTimer(animationTime);
	newExplosion->StartExplosion();

	m_CurrentExplosions.push_back(newExplosion);

	cMesh* explosionMesh = newExplosion->GetMesh();

	g_currentScene->AddMesh(explosionMesh);
}

void cExplosionCreator::Update(double deltaTime)
{
	for (cExplosion* explosion : m_CurrentExplosions)
	{
		explosion->Update(deltaTime);

		if (explosion->IsExplosionOver())
		{
			RemoveExplosion(explosion);
			g_currentScene->DeleteMeshById(explosion->GetMesh()->GetUniqueID());
		}
	}
}

void cExplosionCreator::RemoveExplosion(cExplosion* explosion)
{
	for (int i = (int)m_CurrentExplosions.size() - 1; i >= 0; --i)
	{
		if (m_CurrentExplosions[i]->GetMesh()->GetUniqueID() == explosion->GetMesh()->GetUniqueID())
		{
			m_CurrentExplosions.erase(m_CurrentExplosions.begin() + i);
		}
	}
}
