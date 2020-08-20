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
	char placeholder = map.getEntity(intendedy-1, intendedx);
	for (int i = 0; i < 6; i++)
	{
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
	int x = getPos('x');
	int y = getPos('y');
	for (int i = 0; i < movement; i++)
	{
		if (check == 0 && getPos('x') != 0)
			y--;
		else if (check == 1 && getPos('x') != 79)
			y++;
		else if (check == 2 && getPos('y') != 0)
			x--;
		else if (check == 3 && getPos('y') != 23)
			x++;
		if (collisionCheck(x, y, map) == false)
		{
			setPos('x', x);
			setPos('y', y);
		}
	}
}
