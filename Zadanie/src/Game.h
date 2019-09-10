#pragma once

#include "GameObjects/Player.h"

#include <vector>
#include <map>

class GameObject;
class StaticObject;

class Game
{
public:
	Game();
	~Game();

	enum EGameState
	{
		MAIN_MENU = 0,
		SETTINGS,
		RUNNING,
		LOADING,
		PAUSE_MENU,
		GAME_OVER
	};
	
	static Game& GetInstance();

	void Init();

	void Input(char btn);
	void Update(float deltaTime);

	void SetState(EGameState _state);
	void Reset();

	//getters
	const std::vector<GameObject*>& GetGameObjs() const { return gObjects; }
	const EGameState GetGameState() const { return state; }
	const float GetScore() const { return score; }
	const int GetPlayerShield() const { return player->GetShieldCount(); }
	const int GetPlayerBomb() const { return player->GetBombCount(); }


private:
	EGameState state;

	std::vector<GameObject*> gObjects;
	std::vector<GameObject*> projectiles;

	std::vector<GameObject*> objToDestroy;

	Player* player;

	float score;

	float spawnTime;
	float shootTime;
	float inputTime;

	float enemyCreationRate;

	bool IsCollide(GameObject* gObj1, GameObject* gObj2);
};

