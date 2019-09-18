#include "pch.h"

#include "Settings.h"
#include "Input.h"
#include "Renderer.h"
#include "Game.h"
#include "Texture.h"
#include "Mesh.h"

#include <iostream>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

GLFWwindow* window;

GLFWwindow* init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rocky Space Invaders!", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}

int main(void)
{
	window = init();

	if (!window)
		return -1;

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Settings::GetInstance().ParseConfigFile("res/config.txt");
	Input::Init();

	Game& game = Game::GetInstance();
	game.Init();
	Renderer& renderer = Renderer::GetInstance();
	renderer.Init("res/shaders/basicShader", "res/shaders/textShader", SCR_WIDTH, SCR_HEIGHT);

	TextureManager::GetInstance().LoadTextures();
	MeshManager::GetInstance().LoadModels();

	glfwSetKeyCallback(window, Input::Key_callback);

	// render loop
	double startTime = glfwGetTime();
	double endTime;
	float deltaTime;

	float timer = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (game.GetGameState() == Game::EGameState::LOADING)
		{
			TextureManager::GetInstance().Update();
			MeshManager::GetInstance().Update();

			if (MeshManager::GetInstance().IsLoaded() && TextureManager::GetInstance().IsLoaded())
				Game::GetInstance().SetState(Game::EGameState::MAIN_MENU);
		}

		Input::Handle(window);
		endTime = glfwGetTime();
		deltaTime = float(endTime - startTime);
		startTime = endTime;

		game.Update(deltaTime);

		renderer.Draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}