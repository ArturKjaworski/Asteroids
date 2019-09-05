#include "pch.h"
#include "Player.h"

#include "Projectile.h"
#include "../Settings.h"

#include <iostream>

/*
szybkostrzelność działa[wystrzały / sekundę]
● prędkość pocisków działa[metry / sekundę]
● prędkość “do przodu” statku gracza[metry / sekundę]
● siła ciągu silników manewrowych [niutony]
● masa statku gracza [kilogramy]
*/

Player::Player(const std::string & modelPath, const std::string & texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale):
	GameObject(modelPath, texPath, type, pos, rot, scale)
{
	//take all settings 
	Settings& set = Settings::GetInstance();
	mass = set.settings.playerMass;						
	thrusterForce = set.settings.playerThrusterForce;				
	fireRate = set.settings.playerFireRate;	
	size = glm::vec3(5,0,6);
}

Player::~Player()
{

}

void Player::Move(int op)
{
	if (op%3 == 0)
		(op > 0 ? acceleration.x : acceleration.z) = 0;
	else if (op%2 == 0)
		acceleration.z = (op/2) * thrusterForce / mass;
	else
		acceleration.x = op * thrusterForce / mass;
}

void Player::Shoot()
{

}

void Player::Update(float deltaTime)
{
	velocity += acceleration * deltaTime;

	glm::vec3 pos = transform.GetPos();
	pos += velocity;
	transform.SetPos(pos);
}

void Player::OnHit()
{
	std::cout << "Player hit\n";
}