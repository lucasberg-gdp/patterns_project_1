#include "cPlayerBulletFactory.h"
#include "cPlayerBullet.h"

#include "cScene.h"
#include "cCollisionMediator.h"

extern cScene* g_currentScene;
extern cCollisionMediator* g_CollisionMediator;

//iBullet* cPlayerBulletFactory::CreateBullet(glm::vec3 initialPosition)
//{
//    glm::vec3 bulletSpawnPosition = initialPosition;
//    bulletSpawnPosition.y += 150.0f;
//
//    cPlayerBullet* playerBullet = new cPlayerBullet(bulletSpawnPosition, m_PlayerBulletVelocity, m_PlayerBulletMesh);
//
//    cMesh* bulletMesh = playerBullet->GetBulletMesh();
//    g_currentScene->AddMesh(bulletMesh);
//    g_currentScene->sceneBullets.push_back(playerBullet);
//    g_CollisionMediator->AddBullet(playerBullet);
//    return playerBullet;
//}

void cPlayerBulletFactory::CreateBullet(float x, float y, float z)
{
    glm::vec3 bulletSpawnPosition = glm::vec3(x, y, z);
    bulletSpawnPosition.y += 150.0f;

    cPlayerBullet* playerBullet = new cPlayerBullet(bulletSpawnPosition, m_PlayerBulletSpeed, m_PlayerBulletMesh);

    cMesh* bulletMesh = playerBullet->GetBulletMesh();
    g_currentScene->AddMesh(bulletMesh);
    g_currentScene->sceneBullets.push_back(playerBullet);
    g_CollisionMediator->AddBullet(playerBullet);


    //return playerBullet;

}
