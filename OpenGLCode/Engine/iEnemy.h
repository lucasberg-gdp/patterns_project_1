#pragma once

#include "cMesh.h"

class iEnemy {
	virtual void UpdatePosition(double deltaTime) = 0;
public:
	virtual ~iEnemy() = default;

	virtual bool IsDead() = 0;

	virtual void SetHealth(int health) = 0;
	virtual void DamageEnemy(int damage) = 0;
	virtual void DestroyEnemy() = 0;
	virtual void Update(double deltaTime) = 0;

	virtual std::string GetEnemyType() = 0;
	virtual cMesh* GetEnemyMesh() = 0;

	virtual int GetPoints() = 0;

	virtual void SetIntroType(std::string introType) = 0;
	virtual void Attack() = 0;
	virtual void Skirmish() = 0;

	virtual bool IsInGrid() = 0;
	virtual void SetIsInGrid(bool isInGrid) = 0;
};