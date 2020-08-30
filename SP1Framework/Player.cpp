#include "Player.h"

Player::Player() : inventory{ 0 }
{
	type = TYPE_PLAYER;
	charColor = 0x0A;
	direction = NONE;
}

Player::~Player()
{
	// 
}

void Player::move(Map& map, const double dt)
{
	// empty
}

void Player::movement(Map& map, SKeyEvent* key)
{
	if (key[0].keyDown && pos.Y > 0)
	{
		pos.Y--;
		direction = UP;
	}
	else if (key[2].keyDown && pos.X > 0)
	{
		pos.X--;
		direction = LEFT;
	}
	else if (key[1].keyDown && pos.Y < 25 - 1)
	{
		pos.Y++;
		direction = DOWN;
	}
	else if (key[3].keyDown && pos.X < 80 - 1)
	{
		pos.X++;
		direction = RIGHT;
	}

	int wall = map.getEntity(pos.X, pos.Y - 1);
	if (wall == 'w' || wall=='K' || wall=='H')
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