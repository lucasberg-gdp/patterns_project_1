#pragma once

#include <vector>

#include "cMesh.h"

class cScore
{
	bool m_IsStageComplete = false;
	bool m_IsScoring = false;

	int m_HighestScore = 0;
	int m_CurrentScore = 0;

	int m_TotalScore = 0;

	std::vector<cMesh*> m_ScoreDigits;

	std::vector<std::string> m_DigitsMeshNames;

	glm::vec3 m_ScorePosition = glm::vec3(-1200.0f, 1400.0f, 0.0f);
public:
	cScore();
	~cScore();

	void AddToTotalScore(int score);
	void AddToScore(int score);
	void SetScore(int score);
	void ResetScore();
	
	void SaveScore();
	void StartScoring();

	void RestartScore();

	void Update(double deltaTime);

	bool IsStageComplete();

	void DrawCurrentScore();
};

