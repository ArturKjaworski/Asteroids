#include "Input.h"

#include "Text.h"
#include "Settings.h"

#include <string>
#include <sstream> 
#include <map>

namespace Input 
{
	std::map<int, bool> pressed;

	void Input::Init()
	{
		pressed[GLFW_KEY_W] = false;
		pressed[GLFW_KEY_S] = false;
		pressed[GLFW_KEY_A] = false;
		pressed[GLFW_KEY_D] = false;
		pressed[GLFW_KEY_E] = false;
		pressed[GLFW_KEY_SPACE] = false;
	}

	void Input::Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (!(action == GLFW_PRESS || action == GLFW_RELEASE))
			return;
		
		Game::EGameState state = Game::GetInstance().GetGameState();

		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			if (state == Game::EGameState::MAIN_MENU)
				glfwSetWindowShouldClose(window, true);
			else if (state == Game::EGameState::SETTINGS || state == Game::EGameState::RUNNING)
			{
				if (!TextManager::GetInstance().settingActive)
				{
					Game::GetInstance().SetState(Game::EGameState::MAIN_MENU);
					TextManager::GetInstance().optionSelected = 0;
				}
				TextManager::GetInstance().settingActive = false;
			}
		}

		if (state == Game::EGameState::RUNNING) 
		{
			if (key == GLFW_KEY_E)
			{
				Game::GetInstance().Input('e');
				return;
			}

			pressed[key] = (action == GLFW_PRESS ? true : false);
			return;
		}

		TextManager& text = TextManager::GetInstance();

		if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS)
		{
			//set min and max options for every panel... 
			if (state == Game::EGameState::MAIN_MENU)
				if (text.optionSelected == 0)
					text.optionSelected = 2;
				else
					--text.optionSelected;

			if (state == Game::EGameState::SETTINGS && !text.settingActive)
				if (text.optionSelected == 0)
					text.optionSelected = 10;
				else
					--text.optionSelected;
		}

		if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS)
		{
			if (state == Game::EGameState::MAIN_MENU)
				if (text.optionSelected == 2)
					text.optionSelected = 0;
				else
					++text.optionSelected;

			if (state == Game::EGameState::SETTINGS)
				if (text.optionSelected == 10)
					text.optionSelected = 0;
				else
					++text.optionSelected;
		}

		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			if (state == Game::EGameState::MAIN_MENU)
			{
				switch (text.GetInstance().optionSelected)
				{
				case 0:
				{
					Game::GetInstance().Reset();
					Game::GetInstance().SetState(Game::EGameState::RUNNING);
					break;
				}

				case 1:
				{
					Game::GetInstance().SetState(Game::EGameState::SETTINGS);
					break;
				}

				case 2:
				{
					glfwSetWindowShouldClose(window, true);
					break;
				}
				}
				text.GetInstance().optionSelected = 0;
				return;
			}

			if (state == Game::EGameState::SETTINGS)
			{
				if ((TextManager::GetInstance().optionSelected) == 10)
				{
					Settings::GetInstance().SetDefault();
					return;
				}

				if (!text.settingActive)
					text.settingActive = true;
				else
				{
					Settings& set = Settings::GetInstance();
					switch (TextManager::GetInstance().optionSelected)
					{
						case 0:
						{
							set.settings.playerFireRate = std::stof(text.buffer.empty() ? 0 : text.buffer);
							break;
						}
						case 1:
						{
							set.settings.projectilesSpeed = std::stof(text.buffer.empty() ? 0 : text.buffer);
							break;
						}
						case 2:
						{
							set.settings.forwardSpeed = std::stof(text.buffer.empty() ? 0 : text.buffer);
							break;
						}
						case 3:
						{
							if (text.buffer.empty())
							{
								set.settings.enemyAngVelocityMin.x = 0;
								set.settings.enemyAngVelocityMin.y = 0;
								set.settings.enemyAngVelocityMin.z = 0;
								break;
							}
							std::stringstream ss(text.buffer);
							std::string tmp;

							ss >> tmp;
							set.settings.enemyAngVelocityMin.x = std::stof(tmp);
							ss >> tmp;
							set.settings.enemyAngVelocityMin.y = std::stof(tmp);
							ss >> tmp;
							set.settings.enemyAngVelocityMin.z = std::stof(tmp);

							break;
						}
						case 4:
						{
							if (text.buffer.empty())
							{
								set.settings.enemyAngVelocityMax.x = 0;
								set.settings.enemyAngVelocityMax.y = 0;
								set.settings.enemyAngVelocityMax.z = 0;
								break;
							}

							std::stringstream ss(text.buffer);
							std::string tmp("");

							ss >> tmp;
							set.settings.enemyAngVelocityMax.x = std::stof(tmp);
							ss >> tmp;
							set.settings.enemyAngVelocityMax.y = std::stof(tmp);
							ss >> tmp;
							set.settings.enemyAngVelocityMax.z = std::stof(tmp);
							break;
						}
						case 5:
						{
							set.settings.enemyExplosionTime = std::stof(text.buffer);
							break;
						}
						case 6:
						{
							set.settings.playerThrusterForce = std::stof(text.buffer);
							break;
						}
						case 7:
						{
							set.settings.playerMass = std::stof(text.buffer);
							break;
						}
						case 8:
						{
							set.settings.enemyInitialCreationRate = std::stof(text.buffer);
							break;
						}
						case 9:
						{
							set.settings.enemyCreationRate = std::stof(text.buffer);
							break;
						}
					}
					text.buffer = "";
					text.settingActive = false;
				}
				return;
			}
		}

		if (state == Game::EGameState::GAME_OVER && key == GLFW_KEY_ENTER)
		{
			Game::GetInstance().SetState(Game::EGameState::MAIN_MENU);
		}

		if (text.settingActive && action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_PERIOD || (key>=48 && key <=57) || key == GLFW_KEY_SPACE)
				text.buffer += char(key);
			
			if (key == GLFW_KEY_BACKSPACE)
				if (!text.buffer.empty())
					text.buffer.pop_back();
		}
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