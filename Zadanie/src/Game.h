#pragma once

#include <vector>
#include <map>

class GameObject;
class Player;
class StaticObject;

class Game
{
public:
	Game();
	~Game();

	static Game& GetInstance();

	void Init();

	void Input(char btn);
	void Update(float deltaTime);

	//getters
	const std::vector<GameObject*>& GetGameObjs() const { return gObjects; }

private:
	std::vector<GameObject*> gObjects;
	std::vector<GameObject*> projectiles;

	std::vector<GameObject*> objToDestroy;

	Player* player;

	float spawnTime;
	float shootTime;
	float enemyCreationRate;

	bool IsCollide(GameObject* gObj1, GameObject* gObj2);
};

