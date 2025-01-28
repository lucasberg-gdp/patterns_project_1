#pragma once

#include "cMesh.h"
#include "iEnemy.h"

class cGameGrid
{


public:
	struct GridCell {
		iEnemy* m_EnemyAssigned = nullptr;
		cMesh* m_MeshAssigned = nullptr;
		glm::vec3 m_GridPosition;
		bool m_IsTaken = false;
		bool m_MeshArrived = false;

		GridCell(glm::vec3 position) {
			m_GridPosition = position;
		}
	};


	cGameGrid(){}
	cGameGrid(glm::vec3 centerPosition);

	void CreateBeeGrid();
	void CreateButterflyGrid();
	void CreateMothGrid();

	std::vector<GridCell> m_BeeGrid;
	std::vector<GridCell> m_ButterflyGrid;
	std::vector<GridCell> m_MothGrid;

	float m_DistanceBetweenBees = 200.0f;
	float m_DistanceBetweenButterflies = 200.0f;
	float m_DistanceBetweenMoths = 200.0f;

	glm::vec3 m_GridCenterPosition = glm::vec3(0.0f);

	void ResetGameGrid();

	void Update(double deltaTime);
	void UpdatePositions(double deltaTime);

	void SetCenterPosition(glm::vec3 centerPosition);

	glm::vec3 GetAvailableBeePosition(iEnemy* enemy);
	glm::vec3 GetAvailableButterflyPosition(iEnemy* enemy);
	glm::vec3 GetAvailableMothPosition(iEnemy* enemy);

	glm::vec3 GetAvailablePosition(std::string enemyType, iEnemy* enemy);

	void FillAllCellsWithEnemies();
};

