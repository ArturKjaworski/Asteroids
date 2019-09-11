#pragma once
#include "pch.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

#include <map>

struct Character 
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextManager
{
public:
	TextManager();
	~TextManager();

	static TextManager& GetInstance();

	void Init(GLfloat w, GLfloat h);
	void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	void Render2DText(Shader& textShader);

	int optionSelected;
	bool settingActive;
	std::string buffer;

private:
	void LoadCharacterMap();
	void CreateBuffers();

	void RenderMainMenuText(Shader& textShader);
	void RenderSettingsText(Shader& textShader);
	void RenderRunningText(Shader& textShader);
	void RenderLoadingText(Shader& textShader);
	void RenderGameOverText(Shader& textShader);

	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;

	GLfloat scrW;
	GLfloat scrH;

	glm::vec3 color;
	glm::vec3 selectedColor;	
};

