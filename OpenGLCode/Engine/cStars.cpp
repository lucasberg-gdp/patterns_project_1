#include "cStars.h"

#include "cScene.h"
#include "cColor.h"
#include "MathUtils.h"

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

void cStars::Update(double deltaTime)
{
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
