#pragma once

#include <vector>

class Camera;
class Shader;
class GameObject;

class Renderer
{
public:
	Renderer();
	~Renderer();
	static Renderer& GetInstance();

	void Init(const std::string& shaderName, const GLuint SCR_WIDTH, const GLuint SCR_HEIGHT);

	void Clear();
	void Draw(const std::vector<GameObject*>& gObjs);

private:
	Camera* cam = nullptr;
	Shader* shader = nullptr;

};

