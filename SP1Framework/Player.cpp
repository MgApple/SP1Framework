#include "Player.h"

Player::Player() : inventory{ 0 }
{
	type = TYPE_PLAYER;
	hasSpeedBuff = false;
	charColor = 0x0A;
	direction = NONE;
}

Player::~Player()
{
	// 
}

void Player::setSpeed(bool speedBuff)
{
	this->hasSpeedBuff = speedBuff;
}

bool Player::getSpeed()
{
	return hasSpeedBuff;
}

void Player::move(Map& map, const double dt)
{
	// empty
}

void Player::movement(Map& map, SKeyEvent* key)
{
	if (key[0].keyDown && pos.Y > 0)
	{
		if (hasSpeedBuff)
			pos.Y -= 2;
		else
			pos.Y--;
		direction = UP;
	}
	else if (key[2].keyDown && pos.X > 0)
	{
		if (hasSpeedBuff)
			pos.X -= 2;
		else
			pos.X--;
		direction = LEFT;
	}
	else if (key[1].keyDown && pos.Y < 25 - 1)
	{
		if (hasSpeedBuff)
			pos.Y += 2;
		else
			pos.Y++;
		direction = DOWN;
	}
	else if (key[3].keyDown && pos.X < 80 - 1)
	{
		if (hasSpeedBuff)
			pos.X += 2;
		else
			pos.X++;
		direction = RIGHT;
	}
	if (key[K_SHIFT].keyDown && pos.Y > 1)
		hasSpeedBuff = true;

	int wall = map.getEntity(pos.X, pos.Y - 1);
	if (wall == 'w')
	{
		if (direction == UP)
			pos.Y++;
		else if (direction == LEFT)
			pos.X++;
		else if (direction == DOWN)
			pos.Y--;
		else if (direction == RIGHT)
			pos.X--;
	}
}

enum Player::DIRECTION Player::getDirection()
{
	return direction;
}