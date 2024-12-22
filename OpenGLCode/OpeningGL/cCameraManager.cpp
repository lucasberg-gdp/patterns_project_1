#include "cCameraManager.h"
#include "cFlyCamera.h"
#include "cCamera.h"
#include "cGameManager.h"
#include "cPhysics.h"
#include "cScene.h"

extern cFlyCamera* g_pFlyCamera;
extern cMesh* g_objectSelected;
//extern std::vector< cMesh* > g_vec_pMeshesToDraw;

extern cScene* g_currentScene;
extern iCamera* g_camera;
extern cPhysics* g_pPhysics;

void cCameraManager::LoadCameras(std::vector<sCameraInfo>& cameraInfo,
    std::vector< cGameObject* >& g_gameObjects)
{
    std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

    for (unsigned int i = 0; i < cameraInfo.size(); i++)
    {
        g_currentScene->AddCamera(cCameraManager::CreateCamera(cameraInfo[i]));
    }

    std::vector<iCamera*> sceneCameras = g_currentScene->GetCameras();

    ::g_pFlyCamera = new cFlyCamera(cameraInfo[0]);
    sceneCameras[0] = ::g_pFlyCamera;
    g_objectSelected = meshesToDraw[0];
    g_objectSelected->bIsVisible = false;

    g_camera = new cCamera(cameraInfo[1]);
    g_currentScene->GetCameras()[1] = g_camera;
    g_currentScene->GetCameras()[1]->SetFriendlyName("Camera 1");

    //g_camera = new cCamera(cameraInfo[2]);
    //sceneCameras[2] = g_camera;
    //sceneCameras[2]->SetFriendlyName("Camera 2");

    //g_camera = new cCamera(cameraInfo[3]);
    //sceneCameras[3] = g_camera;
    //sceneCameras[3]->SetFriendlyName("Camera 3");

    //g_camera = new cCamera(cameraInfo[4]);
    //sceneCameras[4] = g_camera;
    //sceneCameras[4]->SetFriendlyName("Camera 4");

    //g_camera = new cCamera(cameraInfo[5]);
    //sceneCameras[5] = g_camera;
    //sceneCameras[5]->SetFriendlyName("Camera 5");
}

iCamera* cCameraManager::CreateCamera(sCameraInfo cameraInfo)
{
    if (cameraInfo.CameraType == "flycamera")
    {
        cFlyCamera* flyCamera = new cFlyCamera(cameraInfo);
        g_currentScene->m_scenePhysics->AddShape(flyCamera->flyCameraPhysics);
        return flyCamera;
    }

    if (cameraInfo.CameraType == "camera")
    {
        return new cCamera(cameraInfo);
    }

    return nullptr;
}
