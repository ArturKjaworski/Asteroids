#include "pch.h"
#include "Game.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/StaticObject.h"
#include "GameObjects/Projectile.h"
#include "Text.h"
#include "Input.h"

#include "Settings.h"

#include <iostream>
#include <fstream>
#include <sstream> 

#include <algorithm>

namespace
{
	const glm::vec3 normals[4] =
	{
		{  0, 0,  1 },
		{ -1, 0,  0 },
		{  0, 0, -1 },
		{  1, 0,  0 }
	};
}

Game::Game()
{
}

Game::~Game()
{
	for (GameObject* obj : gObjects)
		delete(obj);
	gObjects.clear();
	projectiles.clear();
}

Game & Game::GetInstance()
{
	static Game instance;
	return instance;
}

void Game::Init()
{
	state = EGameState::LOADING;
}

void Game::Input(char btn)
{
	TextManager& text = TextManager::GetInstance();

	std::string buffer;

	switch (btn)
	{
	case 'a':
	{
		if (state == EGameState::RUNNING)
			player->Move(-1);
		break;
	}
	case 'w':
	{
		if (state == EGameState::RUNNING)
			player->Move(-2);
		break;
	}
	case 's':
	{
		if (state == EGameState::RUNNING)
			player->Move(2);
		break;
	}
	case 'd':
	{
		if (state == EGameState::RUNNING)
			player->Move(1);
		break;
	}

	case 'u':		//vertical stop
	{
		if (state == EGameState::RUNNING)
			player->Move(-3);
		break;
	}
	case 'l':		//horizontal stop
	{
		if (state == EGameState::RUNNING)
			player->Move(3);
		break;
	}

	case 'f':		//shoot
	{
		if (state == EGameState::RUNNING)
			if (shootTime > (1/player->GetFireRate()))
			{
				glm::vec3 pos = player->GetTransform().GetPos();
				pos.z -= 2;
				Projectile* proj = new Projectile("res/models/projectile.obj", "res/tex/projectile.png", GameObject::EObjectType::PROJECTILE, pos);
				projectiles.push_back(proj);
				gObjects.push_back(proj);

				shootTime = 0;
			}

		break;
	}

	case 'e':
	{
		if (player->Shoot())
		{
			for (GameObject* obj : gObjects)
				if (obj->GetType() == GameObject::EObjectType::ENEMY)
					objToDestroy.push_back(obj);
		}
		break;
	}

	default:
	{
		break;
	}
	};
}

void Game::Update(float deltaTime)
{
	inputTime += deltaTime;

	if (state != EGameState::RUNNING)
		return;

	spawnTime += deltaTime;
	shootTime += deltaTime;

	score += deltaTime;

	int maxEnemyToSpawn = 0;
	if (spawnTime > 1/enemyCreationRate)
	{
		maxEnemyToSpawn = spawnTime / (1/enemyCreationRate);
		spawnTime = spawnTime - (1 / enemyCreationRate);
	}

	for (int ii = 0; ii < maxEnemyToSpawn; ++ii)
		gObjects.push_back(new Enemy("res/models/asteroid1.obj", "res/tex/asteroid1.png", GameObject::EObjectType::ENEMY, glm::vec3((rand()%100 - 50), 0, -80 - (ii * 20))));

	for (GameObject* obj : gObjects)
	{
		obj->Update(deltaTime);
		if (obj->GetType() != GameObject::EObjectType::BG)
		{
			if (obj->GetType() == GameObject::EObjectType::PLAYER)
				continue;

			if (obj->GetType() == GameObject::EObjectType::PROJECTILE)
			{
				if (obj->GetTransform().GetPos().z < -80)
					objToDestroy.push_back(obj);
				continue;
			}

			if (obj->GetType() == GameObject::EObjectType::EXPLOSION)
				if (dynamic_cast<StaticObject*>(obj)->alpha <= 0 || obj->GetTransform().GetPos().z > 70)
					objToDestroy.push_back(obj);

			if (IsCollide(player, obj))
			{
				if (player->OnHit())
					state = EGameState::GAME_OVER;

				objToDestroy.push_back(obj);
				continue;
			}

			if (obj->GetTransform().GetPos().z > 70)
				objToDestroy.push_back(obj);
			else
			{
				//collision with projectiles
				for (auto& proj : projectiles)
				{
					if ((obj->GetTransform().GetPos().x + obj->GetHitBoxSize().x / 2 < proj->GetTransform().GetPos().x - proj->GetHitBoxSize().x / 2) &&
						(obj->GetTransform().GetPos().x - obj->GetHitBoxSize().x / 2 > proj->GetTransform().GetPos().x + proj->GetHitBoxSize().x / 2))
						continue;

					if (IsCollide(proj, obj))
					{
						//proj->OnHit();		in case when projectile got up'ed,  Not implemented
						objToDestroy.push_back(proj);
						objToDestroy.push_back(obj);

						score += 50;
						player->AddBonus();

						enemyCreationRate += Settings::GetInstance().settings.enemyCreationRate;
						continue;
					}
				}
			}
		}
	}
	for (auto& obj : objToDestroy)
	{
		auto tmpObj = std::find(gObjects.begin(), gObjects.end(), obj);
		if (tmpObj != gObjects.end())
		{
			gObjects.erase(tmpObj);
			if (obj->GetType() == GameObject::EObjectType::ENEMY)
				gObjects.push_back(new StaticObject("explo", "res/tex/explo.jpg", GameObject::EObjectType::EXPLOSION, obj->GetTransform().GetPos(), glm::vec3(), glm::vec3(0.1, 0.1, 0.1)));
		}
		else
			continue;	//1 object added 2 times

		if (obj->GetType() == GameObject::EObjectType::PROJECTILE)
		{
			auto tmpProj = std::find(projectiles.begin(), projectiles.end(), obj);
			if (tmpProj != projectiles.end())
				projectiles.erase(tmpProj);
		}

		delete(obj);
		obj = nullptr;
	}
	objToDestroy.clear();
}

void Game::SetState(EGameState _state)
{
	if (state != _state)
		state = _state;
}

void Game::Reset()
{
	score = 0;
	spawnTime = 0;
	shootTime = 0;
	inputTime = 0;
	enemyCreationRate = Settings::GetInstance().settings.enemyInitialCreationRate;

	for (auto& gObj : gObjects)
		delete(gObj);
	gObjects.clear();

	objToDestroy.clear();
	projectiles.clear();

	gObjects.push_back(new StaticObject("BG", "res/tex/BG.jpg", GameObject::EObjectType::BG, glm::vec3(0, 0, 0)));
	player = new Player("res/models/player.obj", "res/tex/player.jpg", GameObject::EObjectType::PLAYER, glm::vec3(0, 0, 60));
	gObjects.push_back(player);

	Input::Init();
}

bool Game::IsCollide(GameObject* gObj1, GameObject* gObj2)
{
	glm::vec3 gObj1Pos = gObj1->GetTransform().GetPos();
	glm::vec3 tmpGObj1Pos = gObj1Pos;

	glm::vec3 gObj2Pos = gObj2->GetTransform().GetPos();
	glm::vec3 tmpGObj2Pos = gObj2Pos;

	bool noresult = false;

	//left bottom (enemy)

	tmpGObj1Pos.x += gObj1->GetHitBoxSize().x / 2;
	tmpGObj1Pos.z -= gObj1->GetHitBoxSize().z / 2;

	tmpGObj2Pos.x -= gObj2->GetHitBoxSize().x / 2;
	tmpGObj2Pos.z += gObj2->GetHitBoxSize().z / 2;

	glm::vec3 vec = tmpGObj1Pos - tmpGObj2Pos;

	float result = glm::dot(normals[0], vec);

	noresult |= (result >= 0 ? true : false);

	//left top (enemy)

	tmpGObj1Pos = gObj1Pos;
	tmpGObj1Pos.x += gObj1->GetHitBoxSize().x / 2;
	tmpGObj1Pos.z += gObj1->GetHitBoxSize().z / 2;

	tmpGObj2Pos = gObj2Pos;
	tmpGObj2Pos.x -= gObj2->GetHitBoxSize().x / 2;
	tmpGObj2Pos.z -= gObj2->GetHitBoxSize().z / 2;

	vec = tmpGObj1Pos - tmpGObj2Pos;

	result = glm::dot(normals[1], vec);
	noresult |= (result >= 0 ? true : false);

	//right top (enemy)

	tmpGObj1Pos = gObj1Pos;
	tmpGObj1Pos.x -= gObj1->GetHitBoxSize().x / 2;
	tmpGObj1Pos.z += gObj1->GetHitBoxSize().z / 2;

	tmpGObj2Pos = gObj2Pos;
	tmpGObj2Pos.x += gObj2->GetHitBoxSize().x / 2;
	tmpGObj2Pos.z -= gObj2->GetHitBoxSize().z / 2;

	vec = tmpGObj1Pos - tmpGObj2Pos;

	result = glm::dot(normals[2], vec);

	noresult |= (result >= 0 ? true : false);

	//right bottom (enemy)

	tmpGObj1Pos = gObj1Pos;
	tmpGObj1Pos.x -= gObj1->GetHitBoxSize().x / 2;
	tmpGObj1Pos.z -= gObj1->GetHitBoxSize().z / 2;

	tmpGObj2Pos = gObj2Pos;
	tmpGObj2Pos.x += gObj2->GetHitBoxSize().x / 2;
	tmpGObj2Pos.z += gObj2->GetHitBoxSize().z / 2;

	vec = tmpGObj1Pos - tmpGObj2Pos;

	result = glm::dot(normals[3], vec);

	noresult |= (result >= 0 ? true : false);

	return !noresult;
}