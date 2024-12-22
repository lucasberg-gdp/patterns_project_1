#pragma once

#include "cMesh.h"

class cGameGrid
{


public:
	struct GridCell {
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

	glm::vec3 GetAvailableBeePosition();
	glm::vec3 GetAvailableButterflyPosition();
	glm::vec3 GetAvailableMothPosition();

	glm::vec3 GetAvailablePosition(std::string enemyType);

	void FillAllCellsWithEnemies();
};

