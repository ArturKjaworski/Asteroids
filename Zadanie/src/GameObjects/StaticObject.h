#pragma once
#include "GameObject.h"

#include <string>

class StaticObject : public GameObject
{
public:
	StaticObject(const std::string& modelPath, const std::string& texPath, EObjectType type, glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
	~StaticObject();

	void Update(float deltaTime)override;
	bool OnHit()override { return true; }

	float scaleTimer;
	float explosionTime;

	float alpha = 0.99;
};

