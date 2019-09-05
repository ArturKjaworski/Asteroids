#include "pch.h"
#include "StaticObject.h"

#include "../Settings.h"

StaticObject::StaticObject(const std::string & modelPath, const std::string & texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale):
	GameObject(modelPath, texPath, type, pos, rot, scale)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::Update(float deltaTime)
{
	MeshManager::GetInstance().AddOffsetToUV(Settings::GetInstance().settings.forwardSpeed * deltaTime);
}
