#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class c2DNavigation
{
public:

	std::vector<glm::vec3> BeeIntroPositions();
	std::vector<glm::vec3> GetInvertedBeeIntro();

	std::vector<glm::vec3> MothAndButterfliesIntroPositions();
	std::vector<glm::vec3> GetInvertedMothAndButterfliesIntro();

	std::vector<glm::vec3> GetBeeSkirmishMovement(glm::vec3 firstPosition);

	std::vector<glm::vec3> GetButterflySkirmishMovement(glm::vec3 firstPosition);
	std::vector<glm::vec3> GetMothSkirmishMovement(glm::vec3 firstPosition);

	// Bezier curve
	std::vector<glm::vec3> GetBeeIntroBezierControlPoints();
	std::vector<glm::vec3> GetBeeRoundBezierControlPoints();


};

