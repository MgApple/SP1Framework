#include "Entity.h"

Entity::Entity(void)
{
	pos.X = -1;
	pos.Y = -1;
	type = TYPE_PLAYER;
	isSliding = false;
	isAttracted = false;
}

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
	else
		return pos.Y;
}

void Entity::setPos(char p, int i)
{
	if (p == 'x')
		pos.X = i;
	else
		pos.Y = i;
}

void Entity::setState(char att, bool state)
{
	if (att == 's')
		isSliding = state;
	else if (att == 'a')
		isAttracted = state;
}

WORD Entity::getCharColor()
{
	return charColor;
}

void Entity::setCharColor(WORD charColor)
{
	this->charColor = charColor;
}