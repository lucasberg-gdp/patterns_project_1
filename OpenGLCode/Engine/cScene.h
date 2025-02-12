#pragma once
#include <vector>

#include "cMesh.h"
#include "cPhysics.h"
#include "cGameObject.h"
#include "cUiManager.h"
#include "cFlyCamera.h"
#include "cLightManager.h"
#include "cPlayer.h"
#include "cSkyBox.h"
#include "iSystem.h"
#include "iBullet.h"
#include "iEnemy.h"
#include "cGameGrid.h"
#include "cScore.h"
#include "cWorldText.h"
#include "cEnemyManager.h"
#include "cCommandGroup.h"
#include "cCommandFactory.h"
#include "cCommandManager.h"

class cScene
{
	std::vector< cMesh* > m_meshes;
	std::vector< iCamera*> m_sceneCameras;

	cUiManager* m_uiManager = nullptr;
	cFlyCamera* m_pFlyCamera = nullptr;
	cSkyBox* m_skyBox = nullptr;

	std::vector< iSystem* > m_systems;

	void UpdateGameObjects(double deltaTime);

public:
	cCommandGroup* m_commandGroup = nullptr;
	cCommandManager* m_commandManager = nullptr;
	cCommandFactory* m_commandFactory = nullptr;

	std::string m_sceneName;

	cPhysics* m_scenePhysics = nullptr;
	std::vector< cGameObject* > m_gameObjects;

	cLightManager* m_sceneLights = nullptr;

	bool isScenePlaying = false;

	unsigned int selectedMeshID = 0;
	unsigned int selectedLightID = 0;
	
	std::vector<iBullet*> sceneBullets;
	std::vector<iEnemy*> enemies;
	cPlayer* m_Player = nullptr;
	cWorldText* worldText = nullptr;
	
	cScore* score = nullptr;
	int stagePoints = 0;

	void RemoveEnemy(iEnemy* enemy);
	void RemoveBullet(iBullet* bullet);

	void ClearStage();

	void PauseGame();
	void ResumeGame();

	cGameGrid gameGrid;

	cScene();
	~cScene();

	cScene(std::string sceneName);

	void Init();

	void Awake(GLFWwindow* window);

	void Start(GLFWwindow* window);
	void StartScene();

	void Update(GLFWwindow* window , double deltaTime);
	void DestroyObjectById(unsigned int id);

	std::vector< cMesh* >& GetAllMeshes();
	void AddMesh(cMesh*& meshToAdd);
	bool DeleteMeshById(unsigned int id);

	cMesh* GetMeshByFriendlyName(std::string friendlyName);
	
	cGameObject* GetPlayer();

	void AddGameObject(cGameObject* gameObjectToAdd);

	bool RemoveMeshByFriendlyName(std::string friendlyName);

	void SortMeshesByName();

	void SelectNextMesh();
	void SelectPreviousMesh();

	void SelectMeshByIndex(unsigned int index);

	void DisplaySceneSkyBox(GLuint shaderProgramID, double deltaTime);

	const char** GetNameOfAllLights();
	unsigned int GetNumberOfLights();

	std::vector<iCamera*>& GetCameras();
	std::vector< cGameObject* >& GetGameObjects();

	bool SaveSceneToJson();

	sSceneInfo SerializeScene();

	cLight GetLightByFriendlyName(std::string friendlyName);

	void AddCamera(iCamera* camera);

	iSystem* GetSystem(std::string systemName);
};