#pragma once
#include "Game.h"

namespace Input
{
	void Init();

	void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void Handle(GLFWwindow* window);
};

