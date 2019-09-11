#include "Player.h"

#include "Projectile.h"
#include "../Settings.h"

Player::Player(const std::string & modelPath, const std::string & texPath, EObjectType type, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale):
	GameObject(modelPath, texPath, type, pos, rot, scale)
{
	Settings& set = Settings::GetInstance();
	mass = set.settings.playerMass;						
	thrusterForce = set.settings.playerThrusterForce;				
	fireRate = set.settings.playerFireRate;	
	size = glm::vec3(5,0,6);

	shields = 0;
	bombs = 0;
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

bool Player::Shoot()
{
	if (bombs > 0)
	{
		--bombs;
		return true;
	}
	return false;
}

void Player::Update(float deltaTime)
{
	velocity += acceleration * deltaTime;

	glm::vec3 pos = transform.GetPos();
	pos += velocity;

	if (pos.x - size.x / 2 < -52)
	{
		pos.x = -52 + size.x/2;
		acceleration.x = 0;
		velocity.x = 0;
	}

	if (pos.x + size.x / 2 > 52)
	{
		pos.x = 52 - size.x / 2;
		acceleration.x = 0;
		velocity.x = 0;
	}

	if (pos.z - size.z / 2 < -70)
	{
		pos.z = -70 + size.z / 2;
		acceleration.z = 0;
		velocity.z = 0;
	}

	if (pos.z + size.z / 2 > 68)
	{
		pos.z = 68 - size.z / 2;
		acceleration.z = 0;
		velocity.z = 0;
	}

	transform.SetPos(pos);
}

bool Player::OnHit()
{
	if (shields > 0)
	{
		--shields;
		return false;
	}

	return true;
}

void Player::AddBonus()
{
	int result = rand() % 100;

	if (result < 10 - 5 * shields)
		AddShield();
	else if (result < 20 - 5 * shields)
		AddBomb();
}

void Player::AddShield()
{
	if (shields < maxShields)
		++shields;
}

void Player::AddBomb()
{
	++bombs;
}