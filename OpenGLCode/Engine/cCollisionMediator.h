#pragma once

#include <vector>

#include "cPlayer.h"
#include "iEnemy.h"
#include "iBullet.h"
#include "cExplosionCreator.h"
#include "cMediaPlayer.h"

class cCollisionMediator
{
	bool m_IsActive = false;

	struct sBulletEnemyCollision 
	{
		iBullet* m_Bullet = nullptr;
		iEnemy* m_Enemy = nullptr;
		glm::vec3 m_CollisionPosition = glm::vec3(0.0f);

		sBulletEnemyCollision(iBullet* bullet, iEnemy* enemy, const glm::vec3& collisionPosition): 
			m_Bullet(bullet), 
			m_Enemy(enemy), 
			m_CollisionPosition(collisionPosition) 
		{}
	};

	struct sBulletPlayerCollision
	{
		iBullet* m_Bullet = nullptr;
		cPlayer* m_Player = nullptr;
		glm::vec3 m_CollisionPosition = glm::vec3(0.0f);

		sBulletPlayerCollision(iBullet* bullet, cPlayer* player, const glm::vec3& collisionPosition) :
			m_Bullet(bullet),
			m_Player(player),
			m_CollisionPosition(collisionPosition)
		{}
	};

	struct sEnemyPlayerCollision
	{
		iEnemy* m_Enemy = nullptr;
		cPlayer* m_Player = nullptr;
		glm::vec3 m_CollisionPosition = glm::vec3(0.0f);

		sEnemyPlayerCollision(iEnemy* enemy, cPlayer* player, const glm::vec3& collisionPosition) :
			m_Enemy(enemy),
			m_Player(player),
			m_CollisionPosition(collisionPosition)
		{}
	};

	cMediaPlayer* m_CollisionExplosionSound = nullptr;

	cPlayer* m_Player = nullptr;
	std::vector<iEnemy*> m_Enemies;
	std::vector<iBullet*> m_Bullets;

	std::vector<sBulletEnemyCollision> m_CurrentBulletEnemyCollisions;
	std::vector<sBulletPlayerCollision> m_CurrentBulletPlayerCollisions;
	std::vector<sEnemyPlayerCollision> m_CurrentEnemyPlayerCollisions;
	
	cExplosionCreator m_ExplosionCreator;

	void UpdateExplosions(double deltaTime);
	void CheckCollisions();

	void CheckCollision(iBullet* bullet, cPlayer* player);
	void CheckCollision(iBullet* bullet, iEnemy* player);
	void CheckCollision(iEnemy* enemy, cPlayer* player);

	void CleanUp();
public:

	cCollisionMediator();
	~cCollisionMediator();

	void Update(double deltaTime);

	void SetPlayer(cPlayer* player);

	void AddEnemy(iEnemy* enemy);
	void AddBullet(iBullet* bullet);

	void RemovePlayer();
	void RemoveEnemy(iEnemy* enemy);
	void RemoveBullet(iBullet* bullet);

	void RemoveAllEnemies();
	void RemoveAllBullets();

	void DestroyPlayer();

	void SetActive(bool isActive);
};