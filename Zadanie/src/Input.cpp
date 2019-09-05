#include "pch.h"
#include "Input.h"


namespace Input 
{
	std::map<int, bool> pressed;


	void Input::Init()
	{
		pressed[GLFW_KEY_W] = false;
		pressed[GLFW_KEY_S] = false;
		pressed[GLFW_KEY_A] = false;
		pressed[GLFW_KEY_D] = false;
		pressed[GLFW_KEY_SPACE] = false;
	}

	void Input::Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (!(action == GLFW_PRESS || action == GLFW_RELEASE))
			return;

		if (key == GLFW_KEY_ESCAPE) 
		{
			glfwSetWindowShouldClose(window, true);
			return;
		}

		pressed[key] = (action == GLFW_PRESS ? true : false);
	}

	void Input::Handle(GLFWwindow* window)
	{
		Game& game = Game::GetInstance();

		for (auto& pair : pressed)
			switch (pair.first)
			{
			case GLFW_KEY_W:
			{
				if (pair.second)
					game.Input('w');
				else if (!pressed[GLFW_KEY_S])
					game.Input('u');

				break;
			}
			case GLFW_KEY_S:
			{
				if (pair.second)
					game.Input('s');
				else if (!pressed[GLFW_KEY_W])
					game.Input('u');

				break;
			}
			case GLFW_KEY_A:
			{
				if (pair.second)
					game.Input('a');
				else
					game.Input('l');

				break;
			}

			case GLFW_KEY_D:
			{
				if (pair.second)
					game.Input('d');
				else if (!pressed[GLFW_KEY_A])
					game.Input('l');
				break;
			}

				case GLFW_KEY_SPACE:
				{
					if (pair.second)
						game.Input('f');	//shoot
					break;
				}
			}
	}
}
