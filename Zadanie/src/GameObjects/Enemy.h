#pragma once
/*
● zakres prędkości kątowej asteroid [radiany / sekundę]
● czas trwania wybuchu asteroidy [sekundy]
*/

#include "GameObject.h"
class Enemy : public GameObject
{
public:
	Enemy(const std::string& modelPath, const std::string& texPath, EObjectType type, glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
	~Enemy();

	void Update(float deltaTime)override;
	void OnHit()override;

private:
	glm::vec3 velocity;
	glm::vec3 angVelocity;
	float explosionTime;
};

