#include "Projectile.h"

#include "../Settings.h"

Projectile::Projectile(const std::string & modelPath, const std::string & texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
	: GameObject(modelPath, texPath, type, pos, rot, scale)
{
	projectileSpeed = Settings::GetInstance().settings.projectilesSpeed;
	size = glm::vec3(1, 0, 3);
}

Projectile::~Projectile()
{
}

void Projectile::Update(float deltaTime)
{
	glm::vec3 pos = transform.GetPos();
	pos.z -= projectileSpeed * deltaTime;

	transform.SetPos(pos);
}

bool Projectile::OnHit()
{
	//if projectile can go through asteroid
	return true;
}
