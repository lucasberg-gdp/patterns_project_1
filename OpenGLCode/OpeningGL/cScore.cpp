#include "cScore.h"
#include "cScene.h"
#include "Engine.h"

extern cScene* g_currentScene;
extern Engine engine;
extern GLuint shaderProgramID;

cScore::cScore()
{
	for (int i = 0; i < 10; i++)
	{
		std::string meshName = "digits/digit" + std::to_string(i) + ".ply";
		m_DigitsMeshNames.push_back(meshName);
	}

	for (int i = 0; i < m_DigitsMeshNames.size(); i++)
	{
		cMesh* digitMesh = new cMesh("digit");
		digitMesh->meshName = m_DigitsMeshNames[i];
		digitMesh->bDoNotLight = true;
		digitMesh->isUsingVertexColors = true;
		digitMesh->bUseDebugColours = false;

		digitMesh->drawPosition = m_ScorePosition;

		m_ScoreDigits.push_back(digitMesh);
	}

	SetScore(0);
}

cScore::~cScore()
{

}

void cScore::AddToTotalScore(int score)
{
	m_TotalScore += score;
}

void cScore::AddToScore(int score)
{
	m_CurrentScore += score;
}

void cScore::SetScore(int score)
{
	m_CurrentScore = score;
}

void cScore::ResetScore()
{
	SetScore(0);
}

void cScore::SaveScore()
{

}

void cScore::StartScoring()
{
	m_IsScoring = true;
}

void cScore::RestartScore()
{
	m_IsScoring = false;
	SetScore(0);
	m_TotalScore = 0;
	m_IsStageComplete = false;
}

void cScore::Update(double deltaTime)
{
	DrawCurrentScore();

	if (m_IsScoring)
	{
		if (m_CurrentScore >= m_TotalScore)
		{
			m_IsStageComplete = true;
		}
	}
}

bool cScore::IsStageComplete()
{
	return m_IsStageComplete;
}

void cScore::DrawCurrentScore()
{
	if (m_CurrentScore == 0)
	{
		engine.DrawObject(m_ScoreDigits[0], glm::mat4(1.0f), shaderProgramID, 0.0);
		
		m_ScoreDigits[0]->drawPosition.x -= 60.0f;
		engine.DrawObject(m_ScoreDigits[0], glm::mat4(1.0f), shaderProgramID, 0.0);

		m_ScoreDigits[0]->drawPosition.x = m_ScorePosition.x;
	}
	else
	{
		std::vector<cMesh*> currentDigitMeshes;

		int currentScore = m_CurrentScore;

		while (currentScore > 1) 
		{
			int rest = currentScore % 10;
			currentScore /= 10;

			currentDigitMeshes.push_back(m_ScoreDigits[rest]);
		} 

		if (currentScore > 0)
		{
			currentDigitMeshes.push_back(m_ScoreDigits[currentScore]);
		}

		for (int i = 0; i < (int)currentDigitMeshes.size(); i++)
		{
			currentDigitMeshes[i]->drawPosition.x -= i * 60.0f;
			engine.DrawObject(currentDigitMeshes[i], glm::mat4(1.0f), shaderProgramID, 0.0);
			currentDigitMeshes[i]->drawPosition.x = m_ScorePosition.x;
		}

		currentDigitMeshes.clear();
	}
}
