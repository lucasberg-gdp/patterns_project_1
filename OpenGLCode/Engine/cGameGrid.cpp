#include "cGameGrid.h"

#include <algorithm>
#include "cColor.h"

#include "Engine.h"

extern Engine engine;

cGameGrid::cGameGrid(glm::vec3 centerPosition)
{
    m_GridCenterPosition = centerPosition;

    // bees 2 x 12
    CreateBeeGrid();

    // butterflies 2 x 12
    CreateButterflyGrid();

    // moths 1 x 12
    CreateMothGrid();
}

void cGameGrid::CreateBeeGrid()
{
    unsigned int numberOfRows = 2;
    unsigned int numberOfColumns = 12;

    for (unsigned int i = 0; i < numberOfRows; i++)
    {
        for (unsigned int j = 0; j < numberOfColumns / 2; j++)
        {
            glm::vec3 gridCellPosition = m_GridCenterPosition;

            gridCellPosition.x -= (m_DistanceBetweenBees / 2.0f) + j * m_DistanceBetweenBees;
            gridCellPosition.y += (m_DistanceBetweenBees / 2.0f) + i * m_DistanceBetweenBees;

            GridCell gridCell(gridCellPosition);
            m_BeeGrid.push_back(gridCell);
        }

        for (unsigned int j = 0; j < numberOfColumns / 2; j++)
        {
            glm::vec3 gridCellPosition = m_GridCenterPosition;

            gridCellPosition.x += (m_DistanceBetweenBees / 2.0f) + j * m_DistanceBetweenBees;
            gridCellPosition.y += (m_DistanceBetweenBees / 2.0f) + i * m_DistanceBetweenBees;

            GridCell gridCell(gridCellPosition);
            m_BeeGrid.push_back(gridCell);
        }
    }

    std::sort(m_BeeGrid.begin(), m_BeeGrid.end(), [this](const GridCell& a, const GridCell& b)
    {
        float distA = std::abs(a.m_GridPosition.x - m_GridCenterPosition.x);
        float distB = std::abs(b.m_GridPosition.x - m_GridCenterPosition.x);
        return distA < distB;
    });
}

void cGameGrid::CreateButterflyGrid()
{
    unsigned int numberOfRows = 2;
    unsigned int numberOfColumns = 12;

    for (unsigned int i = 0; i < numberOfRows; i++)
    {
        for (unsigned int j = 0; j < numberOfColumns / 2; j++)
        {
            glm::vec3 gridCellPosition = m_GridCenterPosition;

            gridCellPosition.y += 400.0f;

            gridCellPosition.x -= (m_DistanceBetweenButterflies / 2.0f) + j * m_DistanceBetweenButterflies;
            gridCellPosition.y += (m_DistanceBetweenButterflies / 2.0f) + i * m_DistanceBetweenButterflies;

            GridCell gridCell(gridCellPosition);
            m_ButterflyGrid.push_back(gridCell);
        }

        for (unsigned int j = 0; j < numberOfColumns / 2; j++)
        {
            glm::vec3 gridCellPosition = m_GridCenterPosition;

            gridCellPosition.y += 400.0f;

            gridCellPosition.x += (m_DistanceBetweenButterflies / 2.0f) + j * m_DistanceBetweenButterflies;
            gridCellPosition.y += (m_DistanceBetweenButterflies / 2.0f) + i * m_DistanceBetweenButterflies;

            GridCell gridCell(gridCellPosition);
            m_ButterflyGrid.push_back(gridCell);
        }
    }

    std::sort(m_ButterflyGrid.begin(), m_ButterflyGrid.end(), [this](const GridCell& a, const GridCell& b)
    {
        float distA = std::abs(a.m_GridPosition.x - m_GridCenterPosition.x);
        float distB = std::abs(b.m_GridPosition.x - m_GridCenterPosition.x);
        return distA < distB;
    });
}

void cGameGrid::CreateMothGrid()
{
    unsigned int numberOfRows = 1;
    unsigned int numberOfColumns = 12;

    for (unsigned int i = 0; i < numberOfRows; i++)
    {
        for (unsigned int j = 0; j < numberOfColumns / 2; j++)
        {
            glm::vec3 gridCellPosition = m_GridCenterPosition;

            gridCellPosition.y += 800.0f;

            gridCellPosition.x -= (m_DistanceBetweenMoths / 2.0f) + j * m_DistanceBetweenMoths;
            gridCellPosition.y += (m_DistanceBetweenMoths / 2.0f) + i * m_DistanceBetweenMoths;

            GridCell gridCell(gridCellPosition);
            m_MothGrid.push_back(gridCell);
        }

        for (unsigned int j = 0; j < numberOfColumns / 2; j++)
        {
            glm::vec3 gridCellPosition = m_GridCenterPosition;

            gridCellPosition.y += 800.0f;

            gridCellPosition.x += (m_DistanceBetweenMoths / 2.0f) + j * m_DistanceBetweenMoths;
            gridCellPosition.y += (m_DistanceBetweenMoths / 2.0f) + i * m_DistanceBetweenMoths;

            GridCell gridCell(gridCellPosition);
            m_MothGrid.push_back(gridCell);
        }
    }

    std::sort(m_MothGrid.begin(), m_MothGrid.end(), [this](const GridCell& a, const GridCell& b)
    {
        float distA = std::abs(a.m_GridPosition.x - m_GridCenterPosition.x);
        float distB = std::abs(b.m_GridPosition.x - m_GridCenterPosition.x);
        return distA < distB;
    });
}

void cGameGrid::ResetGameGrid()
{
    m_BeeGrid.clear();
    // bees 2 x 12
    CreateBeeGrid();

    m_ButterflyGrid.clear();
    // butterflies 2 x 12
    CreateButterflyGrid();

    m_MothGrid.clear();
    // moths 1 x 12
    CreateMothGrid();
}

void cGameGrid::Update(double deltaTime)
{
    UpdatePositions(deltaTime);
}

void cGameGrid::UpdatePositions(double deltaTime)
{

}

void cGameGrid::SetCenterPosition(glm::vec3 centerPosition)
{

}

glm::vec3 cGameGrid::GetAvailableBeePosition(iEnemy* enemy)
{
    for (unsigned int i = 0; i < m_BeeGrid.size(); i++)
    {
        if (!m_BeeGrid[i].m_IsTaken)
        {
            m_BeeGrid[i].m_IsTaken = true;
            return m_BeeGrid[i].m_GridPosition;
        }
    }

    return glm::vec3(0.0f);
}

glm::vec3 cGameGrid::GetAvailableButterflyPosition(iEnemy* enemy)
{
    for (unsigned int i = 0; i < m_ButterflyGrid.size(); i++)
    {
        if (!m_ButterflyGrid[i].m_IsTaken)
        {
            m_ButterflyGrid[i].m_IsTaken = true;
            return m_ButterflyGrid[i].m_GridPosition;
        }
    }

    return glm::vec3(0.0f);
}

glm::vec3 cGameGrid::GetAvailableMothPosition(iEnemy* enemy)
{
    for (unsigned int i = 0; i < m_MothGrid.size(); i++)
    {
        if (!m_MothGrid[i].m_IsTaken)
        {
            m_MothGrid[i].m_IsTaken = true;
            return m_MothGrid[i].m_GridPosition;
        }
    }

    return glm::vec3(0.0f);
}

glm::vec3 cGameGrid::GetAvailablePosition(std::string enemyType, iEnemy* enemy)
{
    if (enemyType == "bee")
    {
        return GetAvailableBeePosition(enemy);
    }

    if (enemyType == "butterfly")
    {
        return GetAvailableButterflyPosition(enemy);
    }

    if (enemyType == "moth")
    {
        return GetAvailableMothPosition(enemy);
    }

    return glm::vec3(0.0f);
}

void cGameGrid::FillAllCellsWithEnemies()
{
    for (unsigned int i = 0; i < m_BeeGrid.size(); i++)
    {
        engine.g_DrawDebugSphere(m_BeeGrid[i].m_GridPosition, 10.0f, cColor::COLOR_RED, 0.0);
    }
    
    for (unsigned int i = 0; i < m_ButterflyGrid.size(); i++)
    {
        engine.g_DrawDebugSphere(m_ButterflyGrid[i].m_GridPosition, 10.0f, cColor::COLOR_BLUE, 0.0);
    }

    for (unsigned int i = 0; i < m_MothGrid.size(); i++)
    {
        engine.g_DrawDebugSphere(m_MothGrid[i].m_GridPosition, 10.0f, cColor::COLOR_WHITE, 0.0);
    }
}
