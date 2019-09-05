#include "pch.h"
#include "GameObject.h"


GameObject::GameObject()
{

}

GameObject::GameObject(const std::string& modelPath, const std::string& texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
	: type(type),
	texPath(texPath),
	modelPath(modelPath)
{
	transform.Init(pos, rot, scale);
}

GameObject::~GameObject()
{
}

void GameObject::Bind() const
{
	MeshManager::GetInstance().Bind(modelPath);
	TextureManager::GetInstance().Bind(texPath);
}

void GameObject::Unbind() const
{
	TextureManager::GetInstance().Unbind();
	MeshManager::GetInstance().Unbind();
}