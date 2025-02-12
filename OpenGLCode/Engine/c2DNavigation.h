#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class c2DNavigation
{
public:
	std::vector<glm::vec3> GetBeeSkirmishMovement(glm::vec3 firstPosition);
	std::vector<glm::vec3> GetBeeSkirmishRoundMovement(glm::vec3 firstPosition);

	std::vector<glm::vec3> GetButterflySkirmishMovement(glm::vec3 firstPosition);
	std::vector<glm::vec3> GetMothSkirmishMovement(glm::vec3 firstPosition);

	std::vector<glm::vec3> GetBeeIntroBezierControlPoints();
	std::vector<glm::vec3> GetBeeRoundBezierControlPoints();

	std::vector<glm::vec3> GetInvertedBeeIntroBezierControlPoints();
	std::vector<glm::vec3> GetInvertedBeeRoundBezierControlPoints();

	std::vector<glm::vec3> GetMothAndButterfliesIntroBezierControlPoints();
	std::vector<glm::vec3> GetMothAndButterfliesRoundBezierControlPoints();

	std::vector<glm::vec3> GetInvertedMothAndButterfliesIntroBezierControlPoints();
	std::vector<glm::vec3> GetInvertedMothAndButterfliesRoundBezierControlPoints();
};

