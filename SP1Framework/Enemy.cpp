#include "Enemy.h"

Enemy::Enemy(TYPE t) : Entity(TYPE::TYPE_ENEMY)
{
	type = t;
}

Enemy::~Enemy()
{
	//blank
}

bool Enemy::collisionCheck(int intendedx, int intendedy,Map &gamemap)
{
	if (gamemap.getEntity(intendedx,intendedy)!=' ')
		return true;
	return false;
}

void Enemy::move(void)
{
	int check = rand() % 4;
	int movement = rand() % 5 + 1;
	int newx = pos.X;
	int newy = pos.Y;
	for (int i = 0; i < movement; i++)
	{
		if (check == 0 && pos.X != 0)
			newx = pos.X--;
		else if (check == 1 && pos.X != 79)
			newx = pos.X++;
		else if (check == 2 && pos.Y != 0)
			newy = pos.Y--;
		else if (check == 3 && pos.Y != 24)
			newy = pos.Y++;
		else
			break;
		if (collisionCheck(newx, newy, gamemap) == false)
		{
			setPos('x', newx);
			setPos('y', newy);
		}

	}
}
