#include "pch.h"
#include "Renderer.h"

#include "GameObjects/GameObject.h"
#include "Camera.h"
#include "Shader.h"

Renderer::Renderer()
{
	
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}

void Renderer::Init(const std::string& shaderName, const GLuint SCR_WIDTH, const GLuint SCR_HEIGHT)
{
	cam = new Camera(glm::vec3(0, 125, 5), 89.0f, float(SCR_HEIGHT) / float(SCR_WIDTH), 0.01f, 1000.f);
	shader = new Shader(shaderName);
}

Renderer::~Renderer()
{
	if (cam != nullptr)
	{
		delete(cam);
		cam = nullptr;
	}
	if (shader != nullptr)
	{
		delete(shader);
		shader = nullptr;
	}
}

void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const std::vector<GameObject*>& gObjs)
{
	shader->Bind();

	for (int ii = 0; ii < gObjs.size(); ++ii)
	{
		//??
		//shader.SetUniform4f("u_Color", gObjs[ii]->color.x, gObjs[ii]->color.y, gObjs[ii]->color.z, 1.0);

		const glm::mat4 modelMat = gObjs[ii]->GetTransform().GetModelMat();
		glm::mat4 proj = cam->GetPerspViewProjection() * modelMat;
		
		shader->SetUniformMat4f("u_Transform", proj);

		gObjs[ii]->Bind();
		glDrawElements(GL_TRIANGLES, gObjs[ii]->GetMesh().GetIndicesCount(), GL_UNSIGNED_INT, 0);

		gObjs[ii]->Unbind();
	}
	shader->Unbind();
}

