#include "cEnemyBulletFactory.h"
#include "cPlayer.h"
#include "cEnemyBullet.h"
#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

//TODO(FRAM): REMOVE THIS
//iBullet* cEnemyBulletFactory::CreateBullet(glm::vec3 initialPosition)
//{
//    glm::vec3 playerPosition = g_currentScene->m_Player->GetPlayerPosition();
//
//    glm::vec3 bulletSpawnPosition = initialPosition;
//    bulletSpawnPosition.y -= 200.0f;
//
//    glm::vec3 directionToPlayer = glm::normalize(playerPosition - bulletSpawnPosition);
//    directionToPlayer *= m_EnemyBulletSpeed;
//
//    iBullet* bullet = new cEnemyBullet(bulletSpawnPosition, directionToPlayer, "enemyBullet.ply");
//
//    cMesh* bulletMesh = bullet->GetBulletMesh();
//
//    float angle = std::atan2(playerPosition.y - bulletSpawnPosition.y, playerPosition.x - bulletSpawnPosition.x);
//
//    float degrees = angle * (float)(180.0 / glm::pi<double>());
//
//    bulletMesh->RotateAroundZ(degrees + 90.0f);
//
//    bulletMesh->drawPosition = bulletSpawnPosition;
//
//    g_currentScene->AddMesh(bulletMesh);
//
//    g_currentScene->sceneBullets.push_back(bullet);
//    g_CollisionMediator->AddBullet(bullet);
//    return bullet;
//}

void cEnemyBulletFactory::CreateBullet(float x, float y, float z)
{
    glm::vec3 playerPosition = g_currentScene->m_Player->GetPlayerPosition();

    glm::vec3 bulletSpawnPosition = glm::vec3(x, y, z);
    bulletSpawnPosition.y -= 200.0f;

    glm::vec3 directionToPlayer = glm::normalize(playerPosition - bulletSpawnPosition);
    directionToPlayer *= m_EnemyBulletSpeed;

    iBullet* bullet = new cEnemyBullet(bulletSpawnPosition, directionToPlayer, "enemyBullet.ply");

    cMesh* bulletMesh = bullet->GetBulletMesh();

    float angle = std::atan2(playerPosition.y - bulletSpawnPosition.y, playerPosition.x - bulletSpawnPosition.x);

    float degrees = angle * (float)(180.0 / glm::pi<double>());

    bulletMesh->RotateAroundZ(degrees + 90.0f);

    bulletMesh->drawPosition = bulletSpawnPosition;

    g_currentScene->AddMesh(bulletMesh);

    g_currentScene->sceneBullets.push_back(bullet);
    g_CollisionMediator->AddBullet(bullet);
    //return bullet;
}
