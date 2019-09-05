#pragma once
#include <string>


struct Conf
{
public:
	float playerFireRate;
	float playerThrusterForce;
	float playerMass;
	float projectilesSpeed;
	float forwardSpeed;
	glm::vec3 enemyAngVelocityMin;
	glm::vec3 enemyAngVelocityMax;
	float enemyExplosionTime;
	float enemyInitialCreationRate;
	float enemyCreationRate;
};

class Settings
{
public:
	Settings();
	~Settings();

	static Settings& GetInstance();

	Conf settings;
	Conf defaultSettings;

	void ParseConfigFile(const std::string& fileName);
	void SetDefault();
	void SaveSettings();
};

