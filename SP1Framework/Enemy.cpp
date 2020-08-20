#include "Enemy.h"

Enemy::Enemy(TYPE t) : Entity(TYPE::TYPE_ENEMY)
{
	type = t;
}

Enemy::~Enemy()
{
	//blank
}

bool Enemy::collisionCheck(int intendedx, int intendedy, Map &map)
{
	bool check = false;
	for (int i = 0; i < 6; i++)
	{//cuz of how the current map is like, this should be rite? idk anymore
		if (map.getEntity(intendedy, intendedx) == collidingCheck[i])
		{
			check = true;
			break;
		}
	}
	return check;
}

void Enemy::move(Map &map)
{
	int check = rand() % 4;
	//int movement = rand() % 5 + 1;
	int movement = 1;
	COORD checker;
	checker.X = getPos('x');
	checker.Y = getPos('y');
	for (int i = 0; i < movement; i++)
	{
		if (check == 0 && getPos('x') != 0)
			checker.X--;
		else if (check == 1 && getPos('x') != 79)
			checker.X++;
		else if (check == 2 && getPos('y') != 0)
			checker.Y--;
		else if (check == 3 && getPos('y') != 24)
			checker.Y++;
		if (collisionCheck(checker.X, checker.Y, map) == false)
		{
			setPos('x', checker.X);
			setPos('y', checker.Y);
		}
	}
}
