#pragma once

#include "GameObject.h"


class Projectile : public GameObject
{
public:
	Projectile(const std::string& modelPath, const std::string& texPath, EObjectType type, glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
	~Projectile();

	void Update(float deltaTime) override;
	bool OnHit() override;

private:
	float projectileSpeed;
};

