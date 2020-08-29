#include "Enemy.h"

Enemy::Enemy(TYPE t) : Entity(TYPE::TYPE_ENEMY)
{
	type = t;
	elapsedTime = 0;
	moveTime = 0.35;
}

Enemy::~Enemy()
{
	//blank
}

bool Enemy::collisionCheck(int intendedx, int intendedy, Map &map)
{
	bool check = false;
	char placeholder = map.getEntity(intendedx, intendedy-1);
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

void Enemy::move(Map& map, const double dt)
{
	elapsedTime += dt;

	int check = rand() % 4;
	//int movement = rand() % 5 + 1;
	int movement = 1;
	int x = getPos('x');
	int y = getPos('y');
	if (elapsedTime > moveTime)
	{
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
				map.setEntity(getPos('x'), getPos('y') - 1, ' ');
				setPos('x', x);
				setPos('y', y);
				if (getType() == 2)
					map.setEntity(x, y - 1, 'K');
				else if (getType() == 3)
					map.setEntity(x, y - 1, 'C');
				else if (getType() == 4)
					map.setEntity(x, y - 1, 'P');
				else if (getType() == 5)
					map.setEntity(x, y - 1, char(4));
				else if (getType() == 6)
					map.setEntity(x, y - 1, 'H');
			}
		}
		elapsedTime = 0.0;
	}
}