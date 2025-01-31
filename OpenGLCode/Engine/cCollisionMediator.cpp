#include "cCollisionMediator.h"
#include <iostream>
#include "cScene.h"

extern cScene* g_currentScene;

cCollisionMediator::cCollisionMediator()
{
	std::vector<std::string> meshAnimations;

	meshAnimations.push_back("explosion1.ply");
	meshAnimations.push_back("explosion2.ply");
	meshAnimations.push_back("explosion3.ply");
	meshAnimations.push_back("explosion4.ply");

	m_ExplosionCreator = cExplosionCreator(meshAnimations);
	m_ExplosionCreator.SetExplosionSpeed(0.1);

	m_CollisionExplosionSound = new cMediaPlayer();
	m_CollisionExplosionSound->Initialize();
}

cCollisionMediator::~cCollisionMediator()
{
	delete m_Player;
}

void cCollisionMediator::Update(double deltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	CheckCollisions();
	UpdateExplosions(deltaTime);

	// Clean up
	CleanUp();
}

void cCollisionMediator::UpdateExplosions(double deltaTime)
{
	m_ExplosionCreator.Update(deltaTime);
}

void cCollisionMediator::SetPlayer(cPlayer* player)
{
	m_Player = player;
}

void cCollisionMediator::AddEnemy(iEnemy* enemy)
{
	m_Enemies.push_back(enemy);
}

void cCollisionMediator::AddBullet(iBullet* bullet)
{
	m_Bullets.push_back(bullet);
}

void cCollisionMediator::RemovePlayer()
{
	m_ExplosionCreator.CreateExplosion(m_Player->GetPlayerPosition());
	m_Player->DisablePlayer();
}

void cCollisionMediator::RemoveEnemy(iEnemy* enemy)
{
	glm::vec3 enemyPosition = enemy->GetEnemyMesh()->drawPosition;

	m_ExplosionCreator.CreateExplosion(enemyPosition);

	g_currentScene->score->AddToScore(enemy->GetPoints());

	for (int i = (int)m_Enemies.size() - 1; i >= 0; --i)
	{
		if (m_Enemies[i]->GetEnemyMesh()->GetUniqueID() == enemy->GetEnemyMesh()->GetUniqueID())
		{
			m_Enemies.erase(m_Enemies.begin() + i);
		}
	}

	g_currentScene->RemoveEnemy(enemy);
}

void cCollisionMediator::RemoveBullet(iBullet* bullet)
{
	for (int i = (int)m_Bullets.size() - 1; i >= 0; --i)
	{
		if (m_Bullets[i]->GetBulletMesh()->GetUniqueID() == bullet->GetBulletMesh()->GetUniqueID())
		{
			m_Bullets.erase(m_Bullets.begin() + i);
		}
	}

	g_currentScene->RemoveBullet(bullet);
}

void cCollisionMediator::RemoveAllEnemies()
{
	m_Enemies.clear();
}

void cCollisionMediator::RemoveAllBullets()
{
	m_Bullets.clear();
}

void cCollisionMediator::DestroyPlayer()
{
	cMesh* playerMesh = m_Player->GetPlayerMesh();

	glm::vec3 playerPosition = playerMesh->drawPosition;

	m_ExplosionCreator.CreateExplosion(playerPosition);
	m_CollisionExplosionSound->PlayAudio("explosionCrunch_000.ogg");
	m_CollisionExplosionSound->SetLooping(false);
	//game.soundsPlaying.push_back(m_PlayerLaserSound);

	playerMesh->bIsVisible = false;
}

void cCollisionMediator::SetActive(bool isActive)
{
	m_IsActive = true;
}

void cCollisionMediator::CheckCollisions()
{
	for (iBullet* bullet : m_Bullets)
	{
		// Check player collision
		if (!m_Player->IsDead())
		{
			CheckCollision(bullet, m_Player);
		}

		// Check enemies collision
		for (iEnemy* enemy : m_Enemies)
		{
			CheckCollision(bullet, enemy);
		}
	}

	if (!m_Player->IsDead())
	{
		for (iEnemy* enemy : m_Enemies)
		{
			CheckCollision(enemy, m_Player);
		}
	}

}

void cCollisionMediator::CheckCollision(iBullet* bullet, cPlayer* player)
{
	glm::vec3 bulletPosition = bullet->GetBulletMesh()->drawPosition;
	glm::vec3 playerPosition = player->GetPlayerMesh()->drawPosition;

	float distance = glm::distance(playerPosition, bulletPosition);

	if (distance < 100.0f && !bullet->IsBulletUsed())
	{
		//std::cout << "iBullet Collided on cPlayer!" << std::endl;

		glm::vec3 collisionPosition = (playerPosition + bulletPosition) / 2.0f;

		sBulletPlayerCollision bulletPlayerCollision(bullet, player, collisionPosition);
		m_CurrentBulletPlayerCollisions.push_back(bulletPlayerCollision);
	}
}

void cCollisionMediator::CheckCollision(iBullet* bullet, iEnemy* enemy)
{
	glm::vec3 enemyPosition = enemy->GetEnemyMesh()->drawPosition;

	if (bullet->GetBulletType() == "playerBullet")
	{
		glm::vec3 bulletPosition = bullet->GetBulletMesh()->drawPosition;

		float distance = glm::distance(enemyPosition, bulletPosition);

		if (distance < 100.0f && !bullet->IsBulletUsed())
		{
			//std::cout << "Collided!" << std::endl;

			glm::vec3 collisionPosition = (enemyPosition + bulletPosition) / 2.0f;

			sBulletEnemyCollision bulletEnemyCollision(bullet, enemy, collisionPosition);
			m_CurrentBulletEnemyCollisions.push_back(bulletEnemyCollision);
		}
	}
}

void cCollisionMediator::CheckCollision(iEnemy* enemy, cPlayer* player)
{
	glm::vec3 enemyPosition = enemy->GetEnemyMesh()->drawPosition;
	glm::vec3 playerPosition = player->GetPlayerMesh()->drawPosition;

	float distance = glm::distance(enemyPosition, playerPosition);

	if (distance < 100.0f)
	{
		//std::cout << "iEnemy Collided on cPlayer!" << std::endl;

		glm::vec3 collisionPosition = (enemyPosition + playerPosition) / 2.0f;

		sEnemyPlayerCollision bulletPlayerCollision(enemy, player, collisionPosition);
		m_CurrentEnemyPlayerCollisions.push_back(bulletPlayerCollision);
	}
}

void cCollisionMediator::CleanUp()
{
	std::vector<sBulletEnemyCollision>::iterator bulletEnemyIterator;
	for (bulletEnemyIterator = m_CurrentBulletEnemyCollisions.begin(); bulletEnemyIterator != m_CurrentBulletEnemyCollisions.end(); ++bulletEnemyIterator)
	{
		iBullet* bullet = (*bulletEnemyIterator).m_Bullet;
		iEnemy* enemy = (*bulletEnemyIterator).m_Enemy;
		glm::vec3 collisionPosition = (*bulletEnemyIterator).m_CollisionPosition;

		if (enemy->GetEnemyMesh()->bIsVisible)
		{
			if (!bullet->IsBulletUsed())
			{
				bullet->BulletHit();
				enemy->DamageEnemy(1);
				m_CollisionExplosionSound->PlayAudio("explosionCrunch_000.ogg");
				m_CollisionExplosionSound->SetLooping(false);
			}
		}
	}

	std::vector<sBulletPlayerCollision>::iterator bulletPlayerIterator;
	for (bulletPlayerIterator = m_CurrentBulletPlayerCollisions.begin(); bulletPlayerIterator != m_CurrentBulletPlayerCollisions.end(); ++bulletPlayerIterator)
	{
		iBullet* bullet = (*bulletPlayerIterator).m_Bullet;
		cPlayer* player = (*bulletPlayerIterator).m_Player;
		glm::vec3 collisionPosition = (*bulletPlayerIterator).m_CollisionPosition;

		bullet->BulletHit();

		if (!player->IsDead())
		{
			player->HitPlayer(1);
			m_CollisionExplosionSound->PlayAudio("explosionCrunch_000.ogg");
			m_CollisionExplosionSound->SetLooping(false);
		}
	}

	std::vector<sEnemyPlayerCollision>::iterator enemyPlayerIterator;
	for (enemyPlayerIterator = m_CurrentEnemyPlayerCollisions.begin(); enemyPlayerIterator != m_CurrentEnemyPlayerCollisions.end(); ++enemyPlayerIterator)
	{
		iEnemy* enemy = (*enemyPlayerIterator).m_Enemy;
		cPlayer* player = (*enemyPlayerIterator).m_Player;
		glm::vec3 collisionPosition = (*enemyPlayerIterator).m_CollisionPosition;

		if (!player->IsDead() && !enemy->IsDead())
		{
			player->HitPlayer(1);
			enemy->DamageEnemy(2);
			m_CollisionExplosionSound->PlayAudio("explosionCrunch_000.ogg");
			m_CollisionExplosionSound->SetLooping(false);
		}
	}

	for (int i = (int)m_Bullets.size() - 1; i >= 0; --i)
	{
		if (m_Bullets[i]->IsBulletUsed())
		{
			m_Bullets.erase(m_Bullets.begin() + i);
		}
	}

	m_CurrentBulletEnemyCollisions.clear();
	m_CurrentBulletPlayerCollisions.clear();
}
