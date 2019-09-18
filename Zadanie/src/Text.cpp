#include "Text.h"

#include "Game.h"
#include "Settings.h"

#include <iostream>

TextManager::TextManager()
{

}

TextManager::~TextManager()
{
}

TextManager & TextManager::GetInstance()
{
	static TextManager instance;
	return instance;
}

void TextManager::Init(GLfloat w, GLfloat h)
{
	scrW = w;
	scrH = h;
	settingActive = false;

	color = glm::vec3(0.0f, 0.5f, 1.0f);
	selectedColor = glm::vec3(0.5f, 0.8f, 0.2f);

	optionSelected = 0;
	LoadCharacterMap();
	CreateBuffers();
}

void TextManager::LoadCharacterMap()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 24);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextManager::CreateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextManager::RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	shader.Bind();
	glUniform3f(glGetUniformLocation(shader.program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextManager::Render2DText(Shader& textShader)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	switch(Game::GetInstance().GetGameState())
	{
		case Game::EGameState::LOADING:
		{
			RenderLoadingText(textShader);
			break;
		}

		case Game::EGameState::MAIN_MENU:
		{
			RenderMainMenuText(textShader);
			break;
		}

		case Game::EGameState::SETTINGS:
		{
			RenderSettingsText(textShader);
			break;
		}

		case Game::EGameState::RUNNING:
		{
			RenderRunningText(textShader);
			break;
		}

		case Game::EGameState::GAME_OVER:
		{
			RenderGameOverText(textShader);
			break;
		}
	} 
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void TextManager::RenderMainMenuText(Shader& textShader)
{
	int counter = 0;
	TextManager::GetInstance().RenderText(textShader, "Start Game ", scrW / 2 - 60, scrH / 2 + 40, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	TextManager::GetInstance().RenderText(textShader, "Settings ", scrW / 2 - 42, scrH / 2, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	TextManager::GetInstance().RenderText(textShader, "Exit ", scrW / 2 - 20, scrH / 2 - 40, 1.0f, (counter++ == optionSelected ? selectedColor : color));
}

void TextManager::RenderSettingsText(Shader& textShader)
{
	Conf& conf = Settings::GetInstance().settings;

	int counter = 0;
	
	RenderText(textShader, "Player fire rate ", 10, scrH - 30, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Projectiles speed ", 10, scrH - 60, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Forward speed ", 10, scrH - 90, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Min enemy angular velocity", 10, scrH - 120, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Max enemy angular velocity", 10, scrH - 150, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Enemy explosion time ", 10, scrH - 180, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Player thrusters force ", 10, scrH - 210, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Player mass ", 10, scrH - 240, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Initial Enemy creation rate ", 10, scrH - 270, 1.0f, (counter++ == optionSelected ? selectedColor : color));
	RenderText(textShader, "Enemy Creation Rate ", 10, scrH - 300, 1.0f, (counter++ == optionSelected ? selectedColor : color));

	RenderText(textShader, "Default", 10, 25, 1.0f, (counter == optionSelected ? selectedColor : color));
	counter = 0;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.playerFireRate), 350, scrH - 30, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.projectilesSpeed), 350, scrH - 60, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.forwardSpeed), 350, scrH - 90, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.enemyAngVelocityMin.x) + " " + std::to_string(conf.enemyAngVelocityMin.y) + " " + std::to_string(conf.enemyAngVelocityMin.z), 350, scrH - 120, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.enemyAngVelocityMax.x) + " " + std::to_string(conf.enemyAngVelocityMax.y) + " " + std::to_string(conf.enemyAngVelocityMax.z), 350, scrH - 150, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.enemyExplosionTime), 350, scrH - 180, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.playerThrusterForce), 350, scrH - 210, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.playerMass), 350, scrH - 240, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.enemyInitialCreationRate), 350, scrH - 270, 1.0f, (counter == optionSelected ? selectedColor : color));
	++counter;
	RenderText(textShader, (settingActive && (counter == optionSelected)) ? buffer : std::to_string(conf.enemyCreationRate), 350, scrH - 300, 1.0f, (counter == optionSelected ? selectedColor : color));

}

void TextManager::RenderRunningText(Shader& textShader)
{
	RenderText(textShader, "Score: ", 10.0f, scrH - 30, 1.0f, color);
	RenderText(textShader, std::to_string(int(Game::GetInstance().GetScore())), 80.0f, scrH - 30, 1.0f, color);

	RenderText(textShader, "Shields: ", 10.0f, scrH - 60, 1.0f, color);
	RenderText(textShader, std::to_string(int(Game::GetInstance().GetPlayerShield())), 100.0f, scrH - 60, 1.0f, color);

	RenderText(textShader, "Bombs: ", 10.0f, scrH - 90, 1.0f, color);
	RenderText(textShader, std::to_string(int(Game::GetInstance().GetPlayerBomb())), 100.0f, scrH - 90, 1.0f, color);
}

void TextManager::RenderLoadingText(Shader& textShader)
{
	RenderText(textShader, "Loading ", scrW / 2 - 42, scrH / 2, 1.0f, color);
}

void TextManager::RenderGameOverText(Shader & textShader)
{
	RenderText(textShader, "Game Over ", scrW / 2 - 50 , scrH / 2 + 40, 1.0f, color);
	RenderText(textShader, "Score: ", scrW / 2 - 40 , scrH / 2, 1.0f, color);
	RenderText(textShader, std::to_string(int(Game::GetInstance().GetScore())), scrW / 2 + 30, scrH / 2, 1.0f, color);
	RenderText(textShader, "Press enter to continue ", scrW / 2 - 125, scrH / 2 - 40, 1.0f, color);
}
