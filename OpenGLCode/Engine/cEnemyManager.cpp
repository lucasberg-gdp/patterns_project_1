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
    if (m_CurrentPositionIndex + 1 >= m_EnemyPositions.size())
    {
        m_FinishedIntroNavigation = true;
        m_IsMovingToGrid = true;
        m_GridPosition = g_currentScene->gameGrid.GetAvailablePosition(m_EnemyType);
        m_EnemyPositions.push_back(m_GridPosition);
        return;
    }

    glm::vec3 currentPosition = m_EnemyMesh->drawPosition;
    glm::vec3 lastPosition = m_EnemyPositions[m_CurrentPositionIndex];
    glm::vec3 nextPosition = m_EnemyPositions[m_CurrentPositionIndex + 1];

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

void cEnemyManager::NavigateToNextPositionOnBezierCurve(double deltaTime)
{
    //if (m_CurrentPositionIndex + 1 >= m_EnemyPositions.size())
    //{
    //    m_FinishedIntroNavigation = true;
    //    m_IsMovingToGrid = true;
    //    m_GridPosition = g_currentScene->gameGrid.GetAvailablePosition(m_EnemyType);
    //    m_EnemyPositions.push_back(m_GridPosition);
    //    return;
    //}

    if (m_ElapsedTime > m_TimeToMove)
    {
        m_IsMakingRound = true;
        m_ElapsedTime = 0.0;
        return;
    }

    double t = m_ElapsedTime / m_TimeToMove;

    double u = 1.0 - t;

    glm::vec3 bezierPosition =
        (float)(u * u * u) * m_IntroBezierControlPoints[0] +                    // (1-t)^3 * P0
        (float)(3 * u * u * t) * m_IntroBezierControlPoints[1] +                // 3(1-t)^2 * t * P1
        (float)(3 * u * t * t) * m_IntroBezierControlPoints[2] +                // 3(1-t) * t^2 * P2
        (float)(t * t * t) * m_IntroBezierControlPoints[3];                     // t^3 * P3

    m_LastPosition = m_EnemyMesh->drawPosition;
    m_EnemyMesh->drawPosition = bezierPosition;
    m_CurrentPosition = bezierPosition;
}

double smoothstep(double edge0, double edge1, double x) {
    x = (x - edge0) / (edge1 - edge0);
    x = glm::clamp(x, 0.0, 1.0);
    return x * x * (3 - 2 * x);
}

glm::vec3 CalculateCircularPosition(float t, glm::vec3 center, float radius, float speed)
{
    float angle = t * speed; // t is the normalized time [0, 1], speed controls rotation speed
    float x = center.x + radius * cos(angle);
    float y = center.y + radius * sin(angle);
    return glm::vec3(x, y, center.z); // Assuming a 2D plane (x, y)
}

void cEnemyManager::MakeRoundOnBezierCurve(double deltaTime)
{
    if (m_ElapsedTime > m_TimeToMakeRound)
    {
        m_EnemyPositions.clear();
        m_EnemyPositions.push_back(m_EnemyMesh->drawPosition);

        m_IsMakingRound = false;
        m_FinishedIntroNavigation = true;
        m_IsMovingToGrid = true;
        m_GridPosition = g_currentScene->gameGrid.GetAvailablePosition(m_EnemyType);
        m_EnemyPositions.push_back(m_GridPosition);
        return;
    }

    double t = m_ElapsedTime / m_TimeToMakeRound;
    double u = 1.0 - t;

    glm::vec3 bezierPosition = glm::vec3(0.0f);

    if (m_RoundBezierControlPoints.size() == 4)
    {
        // Cubic Bezier curve
        bezierPosition =
            (float)(u * u * u) * m_RoundBezierControlPoints[0] + 
            (float)(3 * u * u * t) * m_RoundBezierControlPoints[1] + 
            (float)(3 * u * t * t) * m_RoundBezierControlPoints[2] + 
            (float)(t * t * t) * m_RoundBezierControlPoints[3]; 
    }
    else if (m_RoundBezierControlPoints.size() == 5)
    {
        bezierPosition =
            (float)(u * u * u * u) * m_RoundBezierControlPoints[0] +       
            (float)(4 * u * u * u * t) * m_RoundBezierControlPoints[1] +   
            (float)(6 * u * u * t * t) * m_RoundBezierControlPoints[2] +   
            (float)(4 * u * t * t * t) * m_RoundBezierControlPoints[3] +   
            (float)(t * t * t * t) * m_RoundBezierControlPoints[4];
    }
    else if (m_RoundBezierControlPoints.size() == 6)
    {
        // Quintic Bezier curve
        bezierPosition =
            (float)(u * u * u * u * u) * m_RoundBezierControlPoints[0] +         // (1-t)^5 * P0
            (float)(5 * u * u * u * u * t) * m_RoundBezierControlPoints[1] +     // 5(1-t)^4 * t * P1
            (float)(10 * u * u * u * t * t) * m_RoundBezierControlPoints[2] +    // 10(1-t)^3 * t^2 * P2
            (float)(10 * u * u * t * t * t) * m_RoundBezierControlPoints[3] +    // 10(1-t)^2 * t^3 * P3
            (float)(5 * u * t * t * t * t) * m_RoundBezierControlPoints[4] +     // 5(1-t) * t^4 * P4
            (float)(t * t * t * t * t) * m_RoundBezierControlPoints[5];          // t^5 * P5
    }
    else if (m_RoundBezierControlPoints.size() == 7)
    {
        bezierPosition =
            (float)(u * u * u * u * u * u) * m_RoundBezierControlPoints[0] +         // (1-t)^6 * P0
            (float)(6 * u * u * u * u * u * t) * m_RoundBezierControlPoints[1] +     // 6(1-t)^5 * t * P1
            (float)(15 * u * u * u * u * t * t) * m_RoundBezierControlPoints[2] +    // 15(1-t)^4 * t^2 * P2
            (float)(20 * u * u * u * t * t * t) * m_RoundBezierControlPoints[3] +    // 20(1-t)^3 * t^3 * P3
            (float)(15 * u * u * t * t * t * t) * m_RoundBezierControlPoints[4] +    // 15(1-t)^2 * t^4 * P4
            (float)(6 * u * t * t * t * t * t) * m_RoundBezierControlPoints[5] +     // 6(1-t) * t^5 * P5
            (float)(t * t * t * t * t * t) * m_RoundBezierControlPoints[6];          // t^6 * P6
    }
    else if (m_RoundBezierControlPoints.size() == 8)
    {
        if (t < 0.5)
        {
            t *= 2.0;
            u = 1.0 - t;
            // Cubic Bezier curve
            bezierPosition =
                (float)(u * u * u) * m_RoundBezierControlPoints[0] +
                (float)(3.0 * u * u * t) * m_RoundBezierControlPoints[1] +
                (float)(3.0 * u * t * t) * m_RoundBezierControlPoints[2] +
                (float)(t * t * t) * m_RoundBezierControlPoints[3];
        }
        else
        {
            t -= 0.5;
            t *= 2.0;
            u = 1.0 - t;
            // Cubic Bezier curve
            bezierPosition =
                (float)(u * u * u) * m_RoundBezierControlPoints[4] +
                (float)(3.0 * u * u * t) * m_RoundBezierControlPoints[5] +
                (float)(3.0 * u * t * t) * m_RoundBezierControlPoints[6] +
                (float)(t * t * t) * m_RoundBezierControlPoints[7];
        }
    }

    m_LastPosition = m_EnemyMesh->drawPosition;
    m_EnemyMesh->drawPosition = bezierPosition;
    m_CurrentPosition = bezierPosition;
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
    glm::vec3 lastPosition = m_EnemyPositions[m_CurrentPositionIndex];
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
    if (m_CurrentPositionIndex + 1 >= m_EnemyPositions.size())
    {
        FinishSkirmish();

        return;
    }

    glm::vec3 currentPosition = m_EnemyMesh->drawPosition;
    glm::vec3 lastPosition = m_EnemyPositions[m_CurrentPositionIndex];
    glm::vec3 nextPosition = m_EnemyPositions[m_CurrentPositionIndex + 1];

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
    m_EnemyPositions = m_2DNavigation.BeeIntroPositions();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_EnemyPositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetRightToLeftIntroMovement()
{
    m_EnemyPositions = m_2DNavigation.GetInvertedBeeIntro();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_EnemyPositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetLeftToRightFullCircleIntroMovement()
{
    m_EnemyPositions = m_2DNavigation.MothAndButterfliesIntroPositions();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_EnemyPositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetRightToLeftFullCircleIntroMovement()
{
    m_EnemyPositions = m_2DNavigation.GetInvertedMothAndButterfliesIntro();
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_EnemyPositions[m_CurrentPositionIndex];
}

void cEnemyManager::SetBezierIntroMovement()
{
    m_IntroBezierControlPoints = m_2DNavigation.GetBeeIntroBezierControlPoints();
    m_RoundBezierControlPoints = m_2DNavigation.GetBeeRoundBezierControlPoints();

    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_IntroBezierControlPoints[m_CurrentPositionIndex];
}

void cEnemyManager::SetInvertedBezierIntroMovement()
{
    m_IntroBezierControlPoints = m_2DNavigation.GetInvertedBeeIntroBezierControlPoints();
    m_RoundBezierControlPoints = m_2DNavigation.GetInvertedBeeRoundBezierControlPoints();

    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_IntroBezierControlPoints[m_CurrentPositionIndex];
}

void cEnemyManager::SetBezierIntroLeftRightMovement()
{
    m_IntroBezierControlPoints = m_2DNavigation.GetMothAndButterfliesIntroBezierControlPoints();
    m_RoundBezierControlPoints = m_2DNavigation.GetMothAndButterfliesRoundBezierControlPoints();

    m_TimeToMakeRound = 3.0f;
    m_CurrentPositionIndex = 0;
    m_EnemyMesh->drawPosition = m_IntroBezierControlPoints[m_CurrentPositionIndex];
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
    m_EnemyPositions.clear();

    if (m_EnemyType == "bee")
    {
        m_EnemyPositions = m_2DNavigation.GetBeeSkirmishMovement(m_EnemyMesh->drawPosition);
    }
    else if (m_EnemyType == "butterfly")
    {
        m_EnemyPositions = m_2DNavigation.GetButterflySkirmishMovement(m_EnemyMesh->drawPosition);
    }
    else if (m_EnemyType == "moth")
    {
        m_EnemyPositions = m_2DNavigation.GetMothSkirmishMovement(m_EnemyMesh->drawPosition);
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
    m_EnemyPositions.clear();
    m_EnemyPositions.push_back(m_EnemyMesh->drawPosition);
    m_EnemyPositions.push_back(m_GridPosition);
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
        m_ElapsedTime += deltaTime;

        if (m_IsMakingRound)
        {
            MakeRoundOnBezierCurve(deltaTime);
        }
        else
        {
            NavigateToNextPositionOnBezierCurve(deltaTime);
        }

        //NavigateToNextPosition(deltaTime);
    }
    else
    {
        m_ElapsedTime = 0.0;

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
