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
	char placeholder = map.getEntity(intendedy, intendedx);
	for (int i = 0; i < 6; i++)
	{//the issue's in the map i think either map isnt following the map.text exactly
		//or the conversion into the colours we're using makes a diff position
		if (collidingCheck[i] == placeholder)
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
