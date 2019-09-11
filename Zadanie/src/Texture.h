#pragma once
#include "pch.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

struct Texture
{
	unsigned int id = 0;
	int width = 0;
	int height = 0;
	int BPP = 0;
	unsigned char* localBuffer;
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	
	static TextureManager& GetInstance();

	void LoadTextures();
	//void GenerateTextures();
	void GenerateTexture(int ii);

	void Bind(const std::string& texPath, GLuint slot = 0);
	void Unbind();

	void Update();

	bool IsLoaded();

private:

	std::vector<Texture*> textures;
	std::mutex textures_mutex;

	std::unordered_map<std::string, unsigned int> texID =
	{
		std::make_pair("res/tex/player.jpg", 0)
		, std::make_pair("res/tex/asteroid1.png", 1)
		, std::make_pair("res/tex/projectile.png", 2)
		, std::make_pair("res/tex/BG.jpg", 3)
		, std::make_pair("res/tex/explo.jpg", 4)
	};

	GLuint tex[5];

	unsigned int loadedTexCounter = 0;
};

