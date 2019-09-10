#include "pch.h"
#include "Texture.h"

#include "../stb_img/stb_img.h"

#include <iostream>

#include <thread>

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

void TextureManager::LoadTextures()
{
	glGenTextures(texID.size(), tex);

	std::lock_guard<std::mutex> lock(textures_mutex);
	for (auto& pair : texID)
	{
		std::thread t([=]()
		{
			Texture* tex = new Texture();
			tex->localBuffer = stbi_load(pair.first.c_str(), &tex->width, &tex->height, &tex->BPP, 4);
			tex->id = pair.second;

			if (!tex->localBuffer)
				std::cout << "\nTexture loading failed\n";

			textures.push_back(tex);
		});
		t.detach();
	}
}

void TextureManager::Update()
{
	std::lock_guard<std::mutex> lock(textures_mutex);
	for (int ii = textures.size() - 1; ii >= 0; --ii)
	{
		GenerateTexture(ii);

		delete(textures[ii]);
		textures.erase(textures.begin() + ii);

		++loadedTexCounter;
	}
}

bool TextureManager::IsLoaded()
{
	return loadedTexCounter == texID.size();
}


void TextureManager::GenerateTexture(int ii)
{
	stbi_set_flip_vertically_on_load(1);
	
	glBindTexture(GL_TEXTURE_2D, tex[textures[ii]->id]);
	glBindBuffer(GL_TEXTURE_2D, tex[textures[ii]->id]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textures[ii]->width, textures[ii]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[ii]->localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (textures[ii]->localBuffer)
		stbi_image_free(textures[ii]->localBuffer);
}

void TextureManager::Bind(const std::string& texPath, GLuint slot)
{
	if (slot > 31)
		return;

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex[texID[texPath]]);
}

void TextureManager::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

