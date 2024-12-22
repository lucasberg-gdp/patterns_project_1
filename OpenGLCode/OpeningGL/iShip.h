

class iShip {
	virtual void UpdatePosition(double deltaTime) = 0;
public:
	virtual ~iEnemy() = default;

	virtual void SetHealth(int health) = 0;
	virtual void DamageEnemy(int damage) = 0;
	virtual void DestroyEnemy() = 0;
	virtual void Update(double deltaTime) = 0;

	virtual std::string GetEnemyType() = 0;
	virtual cMesh* GetEnemyMesh() = 0;
};