#include "c2DNavigation.h"
#include <glm/gtc/quaternion.hpp>
#include "MathUtils.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4

std::vector<glm::vec3> c2DNavigation::GetRoundControlPoints(
    glm::vec3 centerPosition, 
    glm::vec3 startPosition, 
    float degrees
) {
    std::vector<glm::vec3> controlPoints;

    if (degrees == 0)
    {
        return controlPoints;
    }
    else if (degrees < 0)
    {
        return MoveClockwise(centerPosition,
            startPosition,
            degrees);
    }
    else if (degrees > 0)
    {
        return MoveCounterClockwise(centerPosition,
            startPosition,
            degrees);
    }
}

std::vector<glm::vec3> c2DNavigation::MoveClockwise(glm::vec3 centerPosition, glm::vec3 startPosition, float degrees)
{
    std::vector<glm::vec3> controlPoints;

    glm::vec3 radiusVector = startPosition - centerPosition;
    float radius = glm::length(radiusVector);

    glm::vec3 direction = glm::normalize(radiusVector);

    float angleRadians = glm::radians(degrees);

    float tangentFactor = (4.0f / 3.0f) * tan(glm::abs(angleRadians) / 4.0f);

    glm::vec3 clockwiseRotation = glm::vec3(0.0f, 0.0f, 1.0f);

    if (degrees < 0.0f)
    {
        clockwiseRotation = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 endPosition = glm::rotate(radiusVector, angleRadians, clockwiseRotation) + centerPosition;

    glm::vec3 tangentStart = glm::normalize(glm::cross(clockwiseRotation, radiusVector));
    glm::vec3 tangentEnd = glm::normalize(glm::cross(clockwiseRotation, centerPosition - endPosition));

    glm::vec3 controlPoint1 = startPosition + tangentFactor * tangentStart * radius;
    glm::vec3 controlPoint2 = endPosition + tangentFactor * tangentEnd * radius;

    controlPoints.push_back(startPosition);
    controlPoints.push_back(controlPoint1);
    controlPoints.push_back(controlPoint2);
    controlPoints.push_back(endPosition);

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::MoveCounterClockwise(glm::vec3 centerPosition, glm::vec3 startPosition, float degrees)
{
    if (degrees <= 180.0f)
    {
        std::vector<glm::vec3> controlPoints;

        glm::vec3 radiusVector = startPosition - centerPosition;
        float radius = glm::length(radiusVector);

        glm::vec3 direction = glm::normalize(radiusVector);

        float angleRadians = glm::radians(degrees);

        float tangentFactor = (4.0f / 3.0f) * tan(angleRadians / 4.0f);

        glm::vec3 counterClockwiseRotation = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::vec3 endPosition = glm::rotate(radiusVector, angleRadians, counterClockwiseRotation) + centerPosition;

        glm::vec3 tangentStart = glm::normalize(glm::cross(counterClockwiseRotation, radiusVector));
        glm::vec3 tangentEnd = glm::normalize(glm::cross(counterClockwiseRotation, centerPosition - endPosition));

        glm::vec3 controlPoint1 = startPosition + tangentFactor * tangentStart * radius;
        glm::vec3 controlPoint2 = endPosition + tangentFactor * tangentEnd * radius;

        controlPoints.push_back(startPosition);
        controlPoints.push_back(controlPoint1);
        controlPoints.push_back(controlPoint2);
        controlPoints.push_back(endPosition);

        return controlPoints;
    }
    else
    {
        std::vector<glm::vec3> controlPoints;

        glm::vec3 radiusVector = startPosition - centerPosition;
        float radius = glm::length(radiusVector);

        glm::vec3 direction = glm::normalize(radiusVector);

        constexpr float angleRadians = glm::radians(180.0f);

        float tangentFactor = (4.0f / 3.0f) * tan(angleRadians / 4.0f);

        glm::vec3 counterClockwiseRotation = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::vec3 endPosition = glm::rotate(radiusVector, angleRadians, counterClockwiseRotation) + centerPosition;

        glm::vec3 tangentStart = glm::normalize(glm::cross(counterClockwiseRotation, radiusVector));
        glm::vec3 tangentEnd = glm::normalize(glm::cross(counterClockwiseRotation, centerPosition - endPosition));

        glm::vec3 controlPoint1 = startPosition + tangentFactor * tangentStart * radius;
        glm::vec3 controlPoint2 = endPosition + tangentFactor * tangentEnd * radius;

        controlPoints.push_back(startPosition);
        controlPoints.push_back(controlPoint1);
        controlPoints.push_back(controlPoint2);
        controlPoints.push_back(endPosition);



        return controlPoints;
    }


}

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
    controlPoints.push_back(glm::vec3(650.0f, -200.f, 0.0f));

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
    glm::vec3 initialPosition = glm::vec3(650.0f, -200.f, 0.0f);
    glm::vec3 centerPosition = glm::vec3(400.0f, -200.f, 0.0f);
    float degreesToRotate = 360.0f;

    //std::vector<glm::vec3> controlPoints = GetRoundControlPoints(centerPosition, initialPosition, degreesToRotate);

    std::vector<glm::vec3> controlPoints;
    controlPoints.push_back(glm::vec3(650.0f, -200.f, 0.0f));
    controlPoints.push_back(glm::vec3(650.0f, -700.f, 0.0f));
    controlPoints.push_back(glm::vec3(100.0f, -700.f, 0.0f));
    controlPoints.push_back(glm::vec3(100.0f, -200.f, 0.0f));

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
    controlPoints.push_back(glm::vec3(-510.0f, -750.f, 0.0f));
    controlPoints.push_back(glm::vec3(-300.0f, -200.f, 0.0f));

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::GetMothAndButterfliesRoundBezierControlPoints()
{
    std::vector<glm::vec3> controlPoints;

    controlPoints.push_back(glm::vec3(-300.0f, -200.f, 0.0f));
    controlPoints.push_back(glm::vec3(-300.0f, 100.f, 0.0f));
    controlPoints.push_back(glm::vec3(-800.0f, 100.f, 0.0f));
    controlPoints.push_back(glm::vec3(-800.0f, -200.f, 0.0f));

    controlPoints.push_back(glm::vec3(-800.0f, -200.f, 0.0f));
    controlPoints.push_back(glm::vec3(-800.0f, -500.f, 0.0f));
    controlPoints.push_back(glm::vec3(-300.0f, -500.f, 0.0f));
    controlPoints.push_back(glm::vec3(-300.0f, -200.f, 0.0f));

    return controlPoints;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedMothAndButterfliesIntroBezierControlPoints()
{
    std::vector<glm::vec3> positions = GetMothAndButterfliesIntroBezierControlPoints();

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        positions[i].x *= -1.0f;
    }

    return positions;
}

std::vector<glm::vec3> c2DNavigation::GetInvertedMothAndButterfliesRoundBezierControlPoints()
{
    std::vector<glm::vec3> positions = GetMothAndButterfliesRoundBezierControlPoints();

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        positions[i].x *= -1.0f;
    }

    return positions;
}
