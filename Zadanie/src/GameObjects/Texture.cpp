#include "pch.h"
#include "Texture.h"

#include "../stb_img/stb_img.h"

#include <iostream>



TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (auto pair : texID)
		glDeleteTextures(1, &pair.second);
}

TextureManager & TextureManager::GetInstance()
{
	static TextureManager instance;
	return instance;
}

void TextureManager::GenerateTextures()
{
	unsigned char* localbuffer;
	int width, height, BPP;

	glGenTextures(texID.size(), tex);
	//create textures
	for (auto& pair : texID)
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* localBuffer = stbi_load(pair.first.c_str(), &width, &height, &BPP, 4);

		if (!localBuffer)
			std::cout << "\nTexture loading failed\n";

		glBindTexture(GL_TEXTURE_2D, tex[pair.second]);
		glBindBuffer(GL_TEXTURE_2D, tex[pair.second]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (localBuffer)
			stbi_image_free(localBuffer);
	}
}

void TextureManager::Bind(const std::string& texPath, GLuint slot)
{
	if (slot > 31)
		return;

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex[texID[texPath]]);
	//glBindBuffer(GL_TEXTURE_2D, tex[texID[texPath]]);
}

void TextureManager::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
