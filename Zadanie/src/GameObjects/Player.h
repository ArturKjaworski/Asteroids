#pragma once
#include "GameObject.h"

#include <vector>

class Projectile;

class Player : public GameObject
{
public:
	Player(const std::string& modelPath, const std::string& texPath, EObjectType type, glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
	~Player();

	void Move(int op);
	bool Shoot();

	void Update(float deltaTime)override;	
	bool OnHit() override;

	const float GetFireRate() const { return fireRate; }
	const int GetBombCount() const { return bombs; }
	const int GetShieldCount() const { return shields; }

	void AddBonus();

private:
	float mass;								//[kg]
	float thrusterForce;					//[N]
	float fireRate;							//[m/s]

	glm::vec3 velocity = glm::vec3();		//[m/s]
	glm::vec3 acceleration = glm::vec3();	//[m/s^2]

	const int maxShields = 2;

	int shields;

	int bombs;

	void AddShield();
	void AddBomb();
};

