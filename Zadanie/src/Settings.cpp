#include "pch.h"
#include "Settings.h"

#include <iostream>
#include <fstream>
#include <sstream> 

Settings::Settings()
{
}

Settings::~Settings()
{
}

Settings & Settings::GetInstance()
{
	static Settings instance;
	return instance;
}

void Settings::ParseConfigFile(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string out;
	std::string line;

	if (!file.is_open())
	{
		std::cout << std::endl << "Unable to load config file " << fileName << std::endl;
		return;
	}

	bool defaultFlag = false;
	std::stringstream ss;

	while (file.good())
	{
		getline(file, line);

		if (std::strcmp(line.c_str(), "default") == 0)
		{
			defaultFlag = true;
			continue;
		}

		ss.str("");
		ss.clear();
		ss << line;

		std::string varName("");

		ss >> varName;

		if (std::strcmp(varName.c_str(), "forwardSpeed") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).forwardSpeed = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "enemyAngVelocityMax") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyAngVelocityMax.x = std::stof(varName);

			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyAngVelocityMax.y = std::stof(varName);

			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyAngVelocityMax.z = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "enemyAngVelocityMin") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyAngVelocityMin.x = std::stof(varName);

			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyAngVelocityMin.y = std::stof(varName);

			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyAngVelocityMin.z = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "enemyCreationRate") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyCreationRate = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "enemyExplosionTime") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyExplosionTime = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "enemyInitialCreationRate") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).enemyInitialCreationRate = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "playerFireRate") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).playerFireRate = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "projectilesSpeed") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).projectilesSpeed = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "playerMass") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).playerMass = std::stof(varName);
		}

		if (std::strcmp(varName.c_str(), "playerThrusterForce") == 0)
		{
			ss >> varName;
			(defaultFlag ? defaultSettings : settings).playerThrusterForce = std::stof(varName);
		}
	}
}

void Settings::SetDefault()
{
	settings = defaultSettings;
}

void Settings::SaveSettings()
{


}
