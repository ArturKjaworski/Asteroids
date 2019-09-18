#include "StaticObject.h"

#include "../Settings.h"

StaticObject::StaticObject(const std::string & modelPath, const std::string & texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale):
	GameObject(modelPath, texPath, type, pos, rot, scale)
{
	if (type == EObjectType::EXPLOSION)
	{
		explosionTime = Settings::GetInstance().settings.enemyExplosionTime;
		scaleTimer = 0;
	}
}

StaticObject::~StaticObject()
{
}

void StaticObject::Update(float deltaTime)
{
	if (type == EObjectType::BG)
	{
		 auto spd = Settings::GetInstance().settings.forwardSpeed / 150;
		MeshManager::GetInstance().AddOffsetToUV(spd* deltaTime);
	}
	if (type == EObjectType::EXPLOSION)
	{
		glm::vec3 pos = transform.GetPos();
		glm::vec3 velo = glm::vec3(0, 0, (Settings::GetInstance().settings.forwardSpeed));
		pos += velo * deltaTime;
		transform.SetPos(pos);

		if (scaleTimer < explosionTime * 0.5)
		{
			scaleTimer += deltaTime;
			glm::vec3 scale = transform.GetScale();
			
			float x = deltaTime / (explosionTime * 0.5) * (1 - scale.x);
			float y = deltaTime / (explosionTime * 0.5) * (1 - scale.y);
			float z = deltaTime / (explosionTime * 0.5) * (1 - scale.z);

			scale += glm::vec3(x, y, z);
			transform.SetScale(scale);
		}
		else if (scaleTimer <= explosionTime)
		{
			scaleTimer += deltaTime;
			
			float x = deltaTime / (explosionTime * 0.5);

			alpha -= x;
		}
	}
}
