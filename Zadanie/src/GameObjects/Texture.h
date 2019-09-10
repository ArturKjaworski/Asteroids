#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	
	static TextureManager& GetInstance();

	void GenerateTextures();

	void Bind(const std::string& texPath, GLuint slot = 0);
	void Unbind();

private:
	std::unordered_map<std::string, unsigned int> texID =
	{
		std::make_pair("res/tex/player.jpg", 0)
		, std::make_pair("res/tex/asteroid1.png", 1)
		, std::make_pair("res/tex/projectile.png", 2)
		, std::make_pair("res/tex/BG.jpg", 3)
		, std::make_pair("res/tex/explo.jpg", 4),
		
	};

	GLuint tex[5];
};

