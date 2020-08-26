#include "Chad.h"

Chad::Chad() : Enemy (TYPE::TYPE_CHAD)
{
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
	charColor = 0x0C;
	isEnd = false;
	target = NULL;
}

Chad::~Chad()
{
	//blank for now
}

void Chad::setPlayer(Entity* target)
{
	this->target = target;
}

bool Chad::checkCollision()
{
	COORD targetPos;
	targetPos.X = target->getPos('x');
	targetPos.Y = target->getPos('y');

	if (targetPos.X == pos.X && targetPos.Y == pos.Y)
		return true;
	else
		return false;
}

