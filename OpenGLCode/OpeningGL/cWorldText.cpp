#include "cWorldText.h"
#include <iostream>
#include "Engine.h"

extern Engine engine;
extern GLuint shaderProgramID;

cWorldText::cWorldText()
{
	FillAlphabet();
	SetText("press enter to start");
}

void cWorldText::FillAlphabet()
{
	for (int i = 'a'; i <= 'z'; i++)
	{
		cMesh* letterMesh = new cMesh("letter");
		letterMesh->meshName = "letters/letter" + std::string(1, std::toupper(static_cast<char>(i))) + ".ply";
		letterMesh->bDoNotLight = true;
		letterMesh->isUsingVertexColors = true;
		letterMesh->bUseDebugColours = false;
		letterMesh->drawPosition = m_TextPosition;

		m_Alphabet.push_back(new sLetter(char(i),letterMesh));
	}
}

void cWorldText::SetText(std::string text)
{
	m_CurrentText = text;

	m_TextPosition.x = (m_CurrentText.size() * m_LetterOffset / 2.0f) * (-1.0f);

	//for (int i = 0; i < m_Alphabet.size(); i++)
	//{
	//	m_Alphabet[i]->m_LetterMesh->drawPosition.x 
	//}
}

void cWorldText::SetText(std::string text, double time)
{

}

void cWorldText::Update(double deltaTime)
{
	DrawCentreText();
}

void cWorldText::DrawCentreText()
{
	std::string currentText = m_CurrentText;

	std::vector<sLetter> currentTextMeshes;

	for (int i = 0; i < m_CurrentText.size(); i++)
	{
		bool foundInAlphabet = false;
		for (int j = 0; j < m_Alphabet.size(); j++)
		{
			if (currentText[i] == m_Alphabet[j]->m_Letter)
			{
				currentTextMeshes.push_back(*m_Alphabet[j]);

				foundInAlphabet = true;
			}
		}

		if (!foundInAlphabet)
		{
			sLetter letter(' ', new cMesh());
			currentTextMeshes.push_back(letter);
		}
	}

	for (int i = 0; i < currentTextMeshes.size(); i++)
	{
		currentTextMeshes[i].m_LetterMesh->drawPosition.x = m_TextPosition.x + (i * m_LetterOffset);
		engine.DrawObject(currentTextMeshes[i].m_LetterMesh, glm::mat4(1.0f), shaderProgramID, 0.0);
		currentTextMeshes[i].m_LetterMesh->drawPosition.x = m_TextPosition.x;
	}
}
