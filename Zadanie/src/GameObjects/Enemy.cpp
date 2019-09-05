#include "pch.h"
#include "Enemy.h"

#include "../Settings.h"

Enemy::Enemy(const std::string & modelPath, const std::string & texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
	: GameObject(modelPath, texPath, type, pos, rot, scale)
{
	Settings& set = Settings::GetInstance();
	
	velocity = glm::vec3(0, 0, float((rand() % 50) + 25) / 100 + set.settings.forwardSpeed);

	glm::vec3 minAng = set.settings.enemyAngVelocityMin;
	glm::vec3 maxAng = set.settings.enemyAngVelocityMax;
	
	angVelocity.x = float(rand()%(int((maxAng.x - minAng.x) * 100) )) / 100 + minAng.x;
	angVelocity.y = float(rand() % (int((maxAng.y - minAng.y) * 100))) / 100 + minAng.y;
	angVelocity.z = float(rand() % (int((maxAng.z - minAng.z) * 100))) / 100 + minAng.z;

	explosionTime = set.settings.enemyExplosionTime;

	size = glm::vec3(3.4, 0, 3.0);
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	glm::vec3 rot = transform.GetRot();
	rot += angVelocity * deltaTime;
	transform.SetRot(rot);
	
	glm::vec3 pos = transform.GetPos();
	pos += velocity;
	transform.SetPos(pos);
}

void Enemy::OnHit()
{

}
