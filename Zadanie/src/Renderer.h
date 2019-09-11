#pragma once
#include "pch.h"

#include <vector>
#include <string>

class Camera;
class Shader;
class GameObject;

class Renderer
{
public:
	Renderer();
	~Renderer();
	static Renderer& GetInstance();

	void Init(const std::string& shaderName, const std::string& textShaderName, const GLfloat SCR_WIDTH, const GLfloat SCR_HEIGHT);

	void Clear();
	void Draw();

private:
	Camera* cam = nullptr;
	Shader* shader = nullptr;
	Shader* textShader = nullptr;

	bool modelLoaded = false;
};

