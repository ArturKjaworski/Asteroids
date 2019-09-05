#include "pch.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Renderer.h"
#include "Game.h"
#include "Input.h"
#include "Settings.h"

#include "gameObjects/Texture.h"
#include "gameObjects/Mesh.h"

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rocky Space Invaders!", NULL, NULL);
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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	window = init();

	glEnable(GL_DEPTH_TEST);

	if (!window)
		return -1;

	Input::Init();
	TextureManager::GetInstance().GenerateTextures();
	MeshManager::GetInstance().LoadModels();

	glfwSetKeyCallback(window, Input::Key_callback);

	Settings::GetInstance().ParseConfigFile("res/config.txt");

	Game& game = Game::GetInstance();
	game.Init();

	Renderer& renderer = Renderer::GetInstance();
	renderer.Init("res/basicShader", SCR_WIDTH, SCR_HEIGHT);

	// render loop
	double startTime = glfwGetTime();
	double endTime;
	float deltaTime;

	while (!glfwWindowShouldClose(window))
	{
		Input::Handle(window);
		endTime = glfwGetTime();
		deltaTime = float(endTime - startTime);
		startTime = endTime;

		game.Update(deltaTime);

		renderer.Clear();
		renderer.Draw(game.GetGameObjs());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}