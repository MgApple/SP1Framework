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