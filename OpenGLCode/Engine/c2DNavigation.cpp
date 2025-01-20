#include "c2DNavigation.h"
#include <glm/gtc/quaternion.hpp>
#include "MathUtils.h"

std::vector<glm::vec3> c2DNavigation::BeeIntroPositions()
{
    std::vector<glm::vec3> positions;

    positions.push_back(glm::vec3(-300.0f, 1500.f, 0.0f));
    positions.push_back(glm::vec3(-300.0f, 1300.f, 0.0f));
    positions.push_back(glm::vec3(0.0f, 500.f, 0.0f));
    positions.push_back(glm::vec3(800.0f, -200.f, 0.0f));

    glm::vec3 centerPosition = glm::vec3(400.0f, -400.0f, 0.0f);

    float initialRadian = 0.0f;
    constexpr float finalRadian = glm::pi<float>();
    float step = 0.05f;
    float radius = 400.0f;

    for (float radian = initialRadian; radian <= finalRadian; radian += step) 
    {
        float x = centerPosition.x + radius * std::cos(radian);
        float y = centerPosition.y - radius * std::sin(radian);

        positions.push_back(glm::vec3(x, y, 0.0f));
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedBeeIntro()
{
    std::vector<glm::vec3> positions = BeeIntroPositions();

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        positions[i].x *= -1.0f;
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::MothAndButterfliesIntroPositions()
{
    std::vector<glm::vec3> positions;

    positions.push_back(glm::vec3(-3000.0f, -1000.f, 0.0f));
    positions.push_back(glm::vec3(-310.0f, -210.f, 0.0f));

    glm::vec3 centerPosition = glm::vec3(-600.0f, -200.0f, 0.0f);

    float initialRadian = 0.0f;
    constexpr float finalRadian = 2.0f * glm::pi<float>();
    float step = 0.2f;
    float radius = 300.0f;

    for (float radian = initialRadian; radian <= finalRadian; radian += step)
    {
        float x = centerPosition.x + radius * std::cos(radian);
        float y = centerPosition.y + radius * std::sin(radian);

        positions.push_back(glm::vec3(x, y, 0.0f));
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedMothAndButterfliesIntro()
{
    std::vector<glm::vec3> positions = MothAndButterfliesIntroPositions();

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        positions[i].x *= -1.0f;
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetBeeSkirmishMovement(glm::vec3 firstPosition)
{
    std::vector<glm::vec3> positions;

    positions.push_back(firstPosition);

    float randomXFinalPosition = MathUtils::GetRandomFloat(-800.0f, 800.0f);
    float yFinalPosition = -1000.0f;

    unsigned int numberOfSubdivisions = 5;
    float radius = 500.0f;

    glm::vec3 finalPosition = glm::vec3(randomXFinalPosition + radius, yFinalPosition + 1.0f, 0.0f);

    glm::vec3 firstToFinalPosition = finalPosition - firstPosition;

    for (unsigned int i = 1; i < numberOfSubdivisions; i++)
    {
        float randomXOffSet = MathUtils::GetRandomFloat(-250.0f, 250.0f);

        float xDirection = (firstToFinalPosition.x / numberOfSubdivisions) * i;
        float yDirection = (firstToFinalPosition.y / numberOfSubdivisions) * i;

        glm::vec3 position = firstPosition + glm::vec3(xDirection + randomXOffSet, yDirection, 0.0f);
        positions.push_back(position);
    }

    glm::vec3 centerPosition = glm::vec3(randomXFinalPosition, yFinalPosition, 0.0f);

    float initialRadian = 0.0f;
    constexpr float finalRadian = glm::pi<float>();
    float step = 0.2f;

    for (float radian = initialRadian; radian <= finalRadian; radian += step)
    {
        float x = centerPosition.x + radius * std::cos(radian);
        float y = centerPosition.y - radius * std::sin(radian);

        positions.push_back(glm::vec3(x, y, 0.0f));
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetButterflySkirmishMovement(glm::vec3 firstPosition)
{
    std::vector<glm::vec3> positions;

    positions.push_back(firstPosition);

    float randomXFinalPosition = MathUtils::GetRandomFloat(-800.0f, 800.0f);
    float yFinalPosition = -2500.0f;

    unsigned int numberOfSubdivisions = 5;
    float radius = 500.0f;

    glm::vec3 finalPosition = glm::vec3(randomXFinalPosition + radius, yFinalPosition + 1.0f, 0.0f);

    glm::vec3 firstToFinalPosition = finalPosition - firstPosition;

    for (unsigned int i = 1; i < numberOfSubdivisions; i++)
    {
        float randomXOffSet = MathUtils::GetRandomFloat(-250.0f, 250.0f);

        float xDirection = (firstToFinalPosition.x / numberOfSubdivisions) * i;
        float yDirection = (firstToFinalPosition.y / numberOfSubdivisions) * i;

        glm::vec3 position = firstPosition + glm::vec3(xDirection + randomXOffSet, yDirection, 0.0f);
        positions.push_back(position);
    }

    glm::vec3 centerPosition = glm::vec3(randomXFinalPosition, yFinalPosition, 0.0f);

    float initialRadian = 0.0f;
    constexpr float finalRadian = glm::pi<float>();
    float step = 0.2f;

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetMothSkirmishMovement(glm::vec3 firstPosition)
{
    std::vector<glm::vec3> positions;

    positions.push_back(firstPosition);

    float randomXFinalPosition = MathUtils::GetRandomFloat(-800.0f, 800.0f);
    float yFinalPosition = -2500.0f;

    unsigned int numberOfSubdivisions = 5;
    float radius = 500.0f;

    glm::vec3 finalPosition = glm::vec3(randomXFinalPosition + radius, yFinalPosition + 1.0f, 0.0f);

    glm::vec3 firstToFinalPosition = finalPosition - firstPosition;

    for (unsigned int i = 1; i < numberOfSubdivisions; i++)
    {
        float randomXOffSet = MathUtils::GetRandomFloat(-250.0f, 250.0f);

        float xDirection = (firstToFinalPosition.x / numberOfSubdivisions) * i;
        float yDirection = (firstToFinalPosition.y / numberOfSubdivisions) * i;

        glm::vec3 position = firstPosition + glm::vec3(xDirection + randomXOffSet, yDirection, 0.0f);
        positions.push_back(position);
    }

    glm::vec3 centerPosition = glm::vec3(randomXFinalPosition, yFinalPosition, 0.0f);

    float initialRadian = 0.0f;
    constexpr float finalRadian = glm::pi<float>();
    float step = 0.2f;

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetBeeIntroBezierControlPoints()
{
    std::vector<glm::vec3> controlPoints;

    controlPoints.push_back(glm::vec3(-200.0f, 1700.f, 0.0f));
    controlPoints.push_back(glm::vec3(-300.0f, 1300.f, 0.0f));
    controlPoints.push_back(glm::vec3(0.0f, 400.f, 0.0f));
    controlPoints.push_back(glm::vec3(600.0f, -200.f, 0.0f));

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedBeeIntroBezierControlPoints()
{
    std::vector<glm::vec3> positions = GetBeeIntroBezierControlPoints();

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        positions[i].x *= -1.0f;
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetBeeRoundBezierControlPoints()
{
    std::vector<glm::vec3> controlPoints;

    controlPoints.push_back(glm::vec3(600.0f, -200.f, 0.0f));
    controlPoints.push_back(glm::vec3(600.0f, -700.f, 0.0f));
    controlPoints.push_back(glm::vec3(100.0f, -700.f, 0.0f));
    controlPoints.push_back(glm::vec3(0.0f, -500.f, 0.0f));
    controlPoints.push_back(glm::vec3(0.0f, -200.f, 0.0f));

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedBeeRoundBezierControlPoints()
{
    std::vector<glm::vec3> positions = GetBeeRoundBezierControlPoints();

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        positions[i].x *= -1.0f;
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetMothAndButterfliesIntroBezierControlPoints()
{
    std::vector<glm::vec3> controlPoints;

    controlPoints.push_back(glm::vec3(-3000.0f, -1000.f, 0.0f));
    controlPoints.push_back(glm::vec3(-2010.0f, -810.f, 0.0f));
    controlPoints.push_back(glm::vec3(-1010.0f, -710.f, 0.0f));
    controlPoints.push_back(glm::vec3(-510.0f, -250.f, 0.0f));
    controlPoints.push_back(glm::vec3(-300.0f, -200.f, 0.0f));

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::GetMothAndButterfliesRoundBezierControlPoints()
{
    std::vector<glm::vec3> controlPoints;

    glm::vec3 centerPosition = glm::vec3(-300.0f, -200.f, 0.0f);
    float radius = 500.0f;

    controlPoints.push_back(glm::vec3(centerPosition.x + radius, centerPosition.y, centerPosition.z));
    controlPoints.push_back(glm::vec3(centerPosition.x + (radius * 0.5f), centerPosition.y + (radius * 0.5f), centerPosition.z));
    controlPoints.push_back(glm::vec3(centerPosition.x - (radius * 0.5f), centerPosition.y + (radius * 0.5f), centerPosition.z));
    controlPoints.push_back(glm::vec3(centerPosition.x - radius, centerPosition.y, centerPosition.z));
    controlPoints.push_back(glm::vec3(centerPosition.x - (radius * 0.5f), centerPosition.y - (radius * 0.5f), centerPosition.z));

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedMothAndButterfliesIntroBezierControlPoints()
{
    return std::vector<glm::vec3>();
}

std::vector<glm::vec3> c2DNavigation::GetInvertedMothAndButterfliesRoundBezierControlPoints()
{
    return std::vector<glm::vec3>();
}
