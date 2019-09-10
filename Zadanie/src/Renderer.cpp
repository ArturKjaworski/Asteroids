#include "pch.h"
#include "Renderer.h"

#include "GameObjects/GameObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Text.h"
#include "GameObjects/Texture.h"
#include "GameObjects/Mesh.h"

#include "GameObjects/StaticObject.h"

#include "Game.h"
#include "Settings.h"

#include "thread"

Renderer::Renderer()
{
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}

void Renderer::Init(const std::string& shaderName, const std::string& textShaderName, const GLfloat SCR_WIDTH, const GLfloat SCR_HEIGHT)
{
	cam = new Camera(glm::vec3(0, 125, 5), 89.0f, SCR_WIDTH, SCR_HEIGHT, 0.01f, 1000.f);
	shader = new Shader(shaderName);
	textShader = new Shader(textShaderName);

	TextManager::GetInstance().Init(SCR_WIDTH, SCR_HEIGHT);

//	Draw();
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

	if (textShader != nullptr)
	{
		delete(textShader);
		textShader = nullptr;
	}
}

void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw()
{
	Clear();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::vector<GameObject*> gObjs = Game::GetInstance().GetGameObjs();

	if (Game::GetInstance().GetGameState() == Game::EGameState::RUNNING)
	{
		shader->Bind();

		for (int ii = 0; ii < gObjs.size(); ++ii)
		{
			if (gObjs[ii]->GetType() == GameObject::EObjectType::EXPLOSION)
				glUniform1f(glGetUniformLocation(shader->program, "alpha"), dynamic_cast<StaticObject*>(gObjs[ii])->alpha);
			else
				glUniform1f(glGetUniformLocation(shader->program, "alpha"), 1.0);

			const glm::mat4 modelMat = gObjs[ii]->GetTransform().GetModelMat();
			glm::mat4 proj = cam->GetPerspViewProjection() * modelMat;

			shader->SetUniformMat4f("u_Transform", proj);

			gObjs[ii]->Bind();
			glDrawElements(GL_TRIANGLES, gObjs[ii]->GetMesh().GetIndicesCount(), GL_UNSIGNED_INT, 0);
			gObjs[ii]->Unbind();
		}
		shader->Unbind();
	}
	//////////////////////////////////////////
	//2D
	glm::mat4 projection = cam->GetOrthoViewProjection();
	textShader->Bind();
	textShader->SetUniformMat4f("projection", projection);
	TextManager::GetInstance().Render2DText(*textShader);
	textShader->Unbind();
}

