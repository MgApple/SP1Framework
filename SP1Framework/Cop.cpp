#include "Cop.h"

Cop::Cop() : Enemy(TYPE::TYPE_COP)
{
	setPos('x', rand() % 79 + 1);
	setPos('y', rand() % 23 + 1);
	charColor = 0x1F;
}

Cop::~Cop()
{
	//nothing
}

void Cop::move()
{
	//overload func, nothing
}

void Cop::setPlayer(Entity* target)
{
	this->target = target;
}

bool Cop::checkCollision()
{
	COORD targetPos;
	targetPos.X = target->getPos('x');
	targetPos.Y = target->getPos('y');

	if (targetPos.X == pos.X && targetPos.Y == pos.Y)
		return true;
	else
		return false;
}