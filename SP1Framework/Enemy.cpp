#include "Enemy.h"

Enemy::Enemy(TYPE t) : Entity(TYPE::TYPE_ENEMY)
{
	type = t;
}

Enemy::~Enemy()
{
	//blank
}

void Enemy::move(void)
{
	int check = rand() % 4;
	int movement = rand() % 5;
	for (int i = 0; i < movement; i++)
	{
		if (check == 0 && pos.X != 0)
			pos.X--;
		else if (check == 1 && pos.X != 79)
			pos.X++;
		else if (check == 2 && pos.Y != 0)
			pos.Y--;
		else if (check == 3 && pos.Y != 79)
			pos.Y++;
		else
			break;
	}
}
