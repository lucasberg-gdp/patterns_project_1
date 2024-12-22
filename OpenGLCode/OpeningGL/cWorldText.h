#pragma once

#include <string>

#include "cMesh.h"

class cWorldText
{
	struct sLetter {
		char m_Letter;
		cMesh* m_LetterMesh;

		sLetter(char letter, cMesh* letterMesh)
		{
			m_Letter = letter;
			m_LetterMesh = letterMesh;
		}
	};

	double m_TextTimer = 0.0;
	double m_TextCooldown = 0.5;

	float m_LetterOffset = 60.0f;

	std::string m_CurrentText;

	std::vector<sLetter*> m_Alphabet;

	glm::vec3 m_TextPosition = glm::vec3(0.0f);

	void FillAlphabet();
public:
	cWorldText();

	void SetText(std::string text);
	void SetText(std::string text, double time);

	void Update(double deltaTime);

	void DrawCentreText();
};

