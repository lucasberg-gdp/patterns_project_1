#include "cStars.h"

#include "cScene.h"
#include "cColor.h"
#include "MathUtils.h"
#include <glm/gtx/easing.hpp>

extern cScene* g_currentScene;

cStars::cStars(unsigned int starsVolume, float descendSpeed)
{
	m_StarsVolume = starsVolume;
	m_DescendSpeed = descendSpeed;

	for (unsigned int i = 0; i < starsVolume; i++)
	{
		cMesh* star = CreateStar();
		star->drawPosition.x = MathUtils::GetRandomFloat(m_MinX, m_MaxX);
		star->drawPosition.y = MathUtils::GetRandomFloat(m_MinY, m_MaxY);

		stars.push_back(star);
		g_currentScene->AddMesh(star);
	}
}

cStars::cStars(unsigned int starsVolume, float slowSpeed, float fastSpeed, double timeToChangeSpeed)
{
	m_StarsVolume = starsVolume;
	m_DescendSpeed = slowSpeed;
	m_SlowDescendSpeed = slowSpeed;
	m_FastDescendSpeed = fastSpeed;
	m_TimeToChangeSpeed = timeToChangeSpeed;

	for (unsigned int i = 0; i < starsVolume; i++)
	{
		cMesh* star = CreateStar();
		star->drawPosition.x = MathUtils::GetRandomFloat(m_MinX, m_MaxX);
		star->drawPosition.y = MathUtils::GetRandomFloat(m_MinY, m_MaxY);

		stars.push_back(star);
		g_currentScene->AddMesh(star);
	}
}

void cStars::Update(double deltaTime)
{
	if (m_IsChangingSpeed)
	{
		m_TimeElapsedToChangeSpeed += deltaTime;

		if (m_IsIncreasingSpeed)
		{
			double percentToDestiny = m_TimeElapsedToChangeSpeed / m_TimeToChangeSpeed;
			float totalVelocity = m_FastDescendSpeed - m_SlowDescendSpeed;

			float easedPercent;

			if (percentToDestiny < 1.0)
			{
				easedPercent = (float)glm::sineEaseIn(percentToDestiny);
				m_DescendSpeed = m_SlowDescendSpeed + easedPercent * totalVelocity;
			}
			else
			{
				m_DescendSpeed = m_FastDescendSpeed;
				m_IsIncreasingSpeed = false;
			}
		}
		else if (m_IsDecreasingSpeed)
		{
			double percentToDestiny = m_TimeElapsedToChangeSpeed / m_TimeToChangeSpeed;
			float totalVelocity = m_SlowDescendSpeed - m_FastDescendSpeed;

			float easedPercent;

			if (percentToDestiny < 1.0)
			{
				easedPercent = (float)glm::sineEaseIn(percentToDestiny);
				m_DescendSpeed = m_FastDescendSpeed + easedPercent * totalVelocity;
			}
			else
			{
				m_DescendSpeed = m_SlowDescendSpeed;
				m_IsDecreasingSpeed = false;
			}
		}

		if (m_TimeToChangeSpeed < m_TimeElapsedToChangeSpeed)
		{
			m_IsChangingSpeed = false;
			m_IsIncreasingSpeed = false;
			m_IsDecreasingSpeed = false;
			m_TimeElapsedToChangeSpeed = 0.0;
		}
	}

	for (unsigned int i = 0; i < m_StarsVolume; i++)
	{
		glm::vec3 starPosition = stars[i]->drawPosition;
		
		if (starPosition.y > m_MinY)
		{
			starPosition.y += m_DescendSpeed * (float)deltaTime;
		}
		else
		{
			starPosition.x = MathUtils::GetRandomFloat(m_MinX, m_MaxX);
			starPosition.y = m_MaxY;
		}

		stars[i]->drawPosition = starPosition;
	}
}

cMesh* cStars::CreateStar()
{
	cMesh* star = new cMesh("star");
	star->setDrawOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	star->bUseDebugColours = true;

	glm::vec4 randomColor = cColor::GetRandomColor();
	star->wholeObjectDebugColourRGBA = randomColor;

	star->drawPosition.z = m_ZPosition;
	star->meshName = "star.ply";
	star->bDoNotLight = true;
	star->isUsingVertexColors = false;

	return star;
}

float cStars::GetTopOfLevel()
{
	return m_MaxY;
}

void cStars::IncreaseSpeed()
{
	if (m_FastDescendSpeed != m_DescendSpeed)
	{
		m_IsChangingSpeed = true;
		m_IsIncreasingSpeed = true;

		m_IsDecreasingSpeed = false;
	}
}

void cStars::DecreaseSpeed()
{
	if (m_SlowDescendSpeed != m_DescendSpeed)
	{
		m_IsChangingSpeed = true;
		m_IsDecreasingSpeed = true;

		m_IsIncreasingSpeed = false;
	}
}
