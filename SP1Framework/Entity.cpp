#include "Entity.h"

Entity::Entity(TYPE t)
{
	pos.X = -1;
	pos.Y = -1;
	type = t;
	isSliding = false;
	isAttracted = false;
}

Entity::~Entity(void)
{
	// empty
}

int Entity::getPos(char p)
{
	if (p == 'x')
		return pos.X;
	else if (p == 'y')
		return pos.Y;
}

void Entity::setPos(char p, int i)
{
	if (p == 'x')
		pos.X = i;
	else if (p == 'y')
		pos.Y = i;
}