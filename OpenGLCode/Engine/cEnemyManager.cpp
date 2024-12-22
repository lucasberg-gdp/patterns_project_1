#include "cEnemyManager.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include "cScene.h"

extern cScene* g_currentScene;

std::vector<glm::vec3> cEnemyManager::GetMovementPositions(unsigned int numberOfPositions, glm::vec3 initialPosition, glm::vec3 finalPosition)
{
    std::vector<glm::vec3> positions;

    for (unsigned int i = 0; i < numberOfPositions; i++)
    {
        glm::vec3 position = initialPosition;

        float x = (finalPosition.x - initialPosition.x) * i;
        float y = (finalPosition.y - initialPosition.y) * i;

        positions.push_back(position);
    }


    return positions;
}

void cEnemyManager::NavigateToNextPosition(double deltaTime)
{
    if (m_CurrentPositionIndex + 1 >= m_BeePositions.size())
    {
        m_FinishedIntroNavigation = true;
        m_IsMovingToGrid = true;
        m_GridPosition = g_currentScene->gameGrid.GetAvailablePosition(m_EnemyType);
        m_BeePositions.push_back(m_GridPosition);
        return;
    }

    glm::vec3 currentPosition = m_EnemyMesh->drawPosition;
    glm::vec3 lastPosition = m_BeePositions[m_CurrentPositionIndex];
    glm::vec3 nextPosition = m_BeePositions[m_CurrentPositionIndex + 1];

    glm::vec3 directionToNext = glm::normalize(nextPosition - lastPosition);

    m_LastPosition = currentPosition;
    m_EnemyMesh->drawPosition += directionToNext * (float)m_EnemySpeed;

    m_CurrentPosition = m_EnemyMesh->drawPosition;

    glm::vec3 toNextPosition = nextPosition - currentPosition;
    glm::vec3 toUpdatedPosition = m_EnemyMesh->drawPosition - currentPosition;

    if (glm::dot(toNextPosition, toUpdatedPosition) < 0.0f) 
    {
        m_CurrentPositionIndex++; 
        m_EnemyMesh->drawPosition = nextPosition;
    }
}

void cEnemyManager::NavigateToGrid(double deltaTime)
{
    if (glm::distance(m_CurrentPosition, m_GridPosition) < 10.0f)
    {
        m_EnemyMesh->setDrawOrientation(glm::quat(1.0f, 0.0f,0.0f,0.0f));
        m_IsInGrid = true;
        m_IsMovingToGrid = false;
        return;
    }

    glm::vec3 currentPosition = m_EnemyMesh->drawPosition;
    glm::vec3 lastPosition = m_BeePositions[m_CurrentPositionIndex];
    glm::vec3 nextPosition = m_GridPosition;

    glm::vec3 directionToNext = glm::normalize(nextPosition - lastPosition);

    m_LastPosition = currentPosition;
    m_EnemyMesh->drawPosition += directionToNext * (float)m_EnemySpeed;

    m_CurrentPosition = m_EnemyMesh->drawPosition;

    glm::vec3 toNextPosition = nextPosition - currentPosition;
    glm::vec3 toUpdatedPosition = m_EnemyMesh->drawPosition - currentPosition;
}

void cEnemyManager::NavigateForSkirmish(double deltaTime)
{
    if (m_CurrentPositionIndex + 1 >= m_BeePositions.size())
    {
        FinishSkirmish();

        return;
    }

    glm::vec3 currentPosition = m_EnemyMesh->drawPosition;
    glm::vec3 lastPosition = m_BeePositions[m_CurrentPositionIndex];
    glm::vec3 nextPosition = m_BeePositions[m_CurrentPositionIndex + 1];

    glm::vec3 directionToNext = glm::normalize(nextPosition - lastPosition);

    m_LastPosition = currentPosition;
    m_EnemyMesh->drawPosition += directionToNext * (float)m_EnemySpeed;

    m_CurrentPosition = m_EnemyMesh->drawPosition;

    glm::vec3 toNextPosition = nextPosition - currentPosition;
    glm::vec3 toUpdatedPosition = m_EnemyMesh->drawPosition - currentPosition;

    if (glm::dot(toNextPosition, toUpdatedPosition) < 0.0f)
    {
        m_CurrentPositionIndex++;
        m_EnemyMesh->drawPosition = nextPosition;
    }
}

cEnemyManager::cEnemyManager(std::string enemyType)
{
    m_EnemyType = enemyType;
}

cEnemyManager::cEnemyManager(std::string enemyType, cMesh* mesh, glm::vec3 initialPosition, glm::vec3 finalPosition, double enemySpeed):
    m_EnemyMesh(mesh),
    m_InitialPosition(initialPosition),
    m_FinalPosition(finalPosition),
    m_EnemySpeed(enemySpeed),
    m_LastPosition(initialPosition),
    m_CurrentPosition(initialPosition),
    m_XStartPosition(initialPosition.x),
    m_EnemyType(enemyType)
{

}

void cEnemyManager::SetLeftToRightIntroMovement()
{
    m_BeePositions = m_2DNavigation.BeeIntroPositions();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_BeePositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetRightToLeftIntroMovement()
{
    m_BeePositions = m_2DNavigation.GetInvertedBeeIntro();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_BeePositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetLeftToRightFullCircleIntroMovement()
{
    m_BeePositions = m_2DNavigation.MothAndButterfliesIntroPositions();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_BeePositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetRightToLeftFullCircleIntroMovement()
{
    m_BeePositions = m_2DNavigation.GetInvertedMothAndButterfliesIntro();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_BeePositions[m_CurrentPositionIndex];
}

void cEnemyManager::StartMoving(glm::vec3 position)
{
    m_XStartPosition = position.x;
    m_InitialPosition = position;
    m_IsMoving = true;
    m_CurrentPosition = m_InitialPosition;
    m_EnemyMesh->drawPosition = m_CurrentPosition;
}

void cEnemyManager::StartMovingToGrid()
{

}

void cEnemyManager::StartSkirmishing()
{
    m_EnemySpeed = 20.0f;
    m_BeePositions.clear();

    if (m_EnemyType == "bee")
    {
        m_BeePositions = m_2DNavigation.GetBeeSkirmishMovement(m_EnemyMesh->drawPosition);
    }
    else if (m_EnemyType == "butterfly")
    {
        m_BeePositions = m_2DNavigation.GetButterflySkirmishMovement(m_EnemyMesh->drawPosition);
    }
    else if (m_EnemyType == "moth")
    {
        m_BeePositions = m_2DNavigation.GetMothSkirmishMovement(m_EnemyMesh->drawPosition);
    }

    m_XStartPosition = m_EnemyMesh->drawPosition.x;
    m_InitialPosition = m_EnemyMesh->drawPosition;
    m_IsSkirmishing = true;
    m_FinishedIntroNavigation = true;
    m_IsMoving = true;
    m_CurrentPosition = m_InitialPosition;
    m_CurrentPositionIndex = 0;
    m_IsInGrid = false;
}

void cEnemyManager::FinishSkirmish()
{
    if (m_EnemyType == "butterfly" || m_EnemyType == "moth")
    {
        m_EnemyMesh->drawPosition.y = 2000.0f;
    }

    m_IsMovingToGrid = true;
    m_BeePositions.clear();
    m_BeePositions.push_back(m_EnemyMesh->drawPosition);
    m_BeePositions.push_back(m_GridPosition);
    m_CurrentPositionIndex = 0;
    m_IsInGrid = false;
    m_IsSkirmishing = false;
}

void cEnemyManager::SelectMovementType(movementType movementType)
{
    m_MovementType = movementType;
}

void cEnemyManager::Update(double deltaTime)
{
    UpdatePosition(deltaTime);

    if (m_FaceMovementDirection && !m_IsInGrid)
    {
        FaceMovementDirection();
    }
}

void cEnemyManager::UpdatePosition(double deltaTime)
{
    if (!m_IsMoving)
    {
        return;
    }

    if (!m_FinishedIntroNavigation)
    {
        NavigateToNextPosition(deltaTime);
    }
    else
    {
        if (m_IsMovingToGrid)
        {
            NavigateToGrid(deltaTime);
        }
        else if (m_IsSkirmishing)
        {
            NavigateForSkirmish(deltaTime);
        }

        if (glm::distance(m_GridPosition, m_CurrentPosition) < 1.0f)
        {
            m_EnemyMesh->setRotationFromEuler(glm::vec3(0.0f));
        }
    }

}

glm::vec3 cEnemyManager::GetPositionOnLine(double deltaTime)
{
    if (glm::distance(m_FinalPosition, m_CurrentPosition) < 1.0f)
    {
        ArrivedAtDestination();
    }

    m_LastPosition = m_CurrentPosition;

    glm::vec3 direction = glm::normalize(m_FinalPosition - m_CurrentPosition);

    m_CurrentPosition.x += direction.x * (float)(deltaTime * m_EnemySpeed) + m_InitialPosition.x;
    m_CurrentPosition.y += direction.y * (float)(deltaTime * m_EnemySpeed);

    return m_CurrentPosition;
}

glm::vec3 cEnemyManager::GetPositionOnSine(double deltaTime)
{
    if (m_FinalPosition.y > m_CurrentPosition.y) 
    {
        ArrivedAtDestination();
    }

    float frequency = 0.01f;
    float amplitude = 400.0f;

    m_YAdvancedPosition += (float)(deltaTime * m_EnemySpeed * 10.0);

    m_LastPosition = m_CurrentPosition;

    m_CurrentPosition.y -= (float)(deltaTime * m_EnemySpeed * 10.0f);

    m_CurrentPosition.x = amplitude * glm::sin(frequency * m_YAdvancedPosition) + m_XStartPosition;

    return m_CurrentPosition;
}

void cEnemyManager::FaceMovementDirection()
{
    float angle = std::atan2(
        m_LastPosition.y - m_CurrentPosition.y ,
        m_LastPosition.x - m_CurrentPosition.x
    );

    float degrees = glm::degrees(angle);

    if (degrees < 0.0f) 
    {
        degrees += 360.0f;
    }

    m_EnemyMesh->drawOrientation.z = degrees + 90.0f;
    m_EnemyMesh->setRotationFromEuler(glm::radians(m_EnemyMesh->drawOrientation));
}

void cEnemyManager::ArrivedAtDestination()
{
    m_IsMoving = false;
}

void cEnemyManager::SkipIntro()
{
    m_FinishedIntroNavigation = true;
}
