#include "Entity.h"

Entity::Entity(void)
{
	pos.X = -1;
	pos.Y = -1;
	type = TYPE_PLAYER;
	isHoldingTP = false;
	charColor = 0xFF;
}

Entity::Entity(TYPE t)
{
	pos.X = -1;
	pos.Y = -1;
	type = t;
	isHoldingTP = false;
	charColor = 0xFF;
}

Entity::~Entity(void)
{
	// empty
}

int Entity::getType(void)
{
	return type;
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

void Entity::setState(bool state)
{
	isHoldingTP = state;
}

bool Entity::getState(void)
{
	return isHoldingTP;
}

WORD Entity::getCharColor()
{
	return charColor;
}

void Entity::setCharColor(WORD charColor)
{
	this->charColor = charColor;
}

void Entity::reLoc(void)
{
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
}