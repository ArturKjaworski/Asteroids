#pragma once
#include "../pch.h"

#include "Transform.h"

#include "../Mesh.h"
#include "../Texture.h"

#include <string>

class GameObject
{
public:
	enum EObjectType
	{
		  BG = 0
		, PLAYER
		, ENEMY
		, PROJECTILE
		, EXPLOSION
	};

	GameObject();
	GameObject(const std::string& modelPath, const std::string& texPath, EObjectType type, glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));

	virtual ~GameObject();

	void Bind() const;
	void Unbind() const;

	virtual void Update(float deltaTime) = 0;
	virtual bool OnHit() = 0;

	//getters
	Transform& GetTransform() { return transform; }
	const Mesh& GetMesh() { return MeshManager::GetInstance().GetMesh(modelPath); }

	const EObjectType GetType() const { return type; }
	const glm::vec3& GetHitBoxSize() const { return size; }

protected:
	
	glm::vec3 size;

	EObjectType type;
	Transform transform;

	std::string texPath;
	std::string modelPath;
};

