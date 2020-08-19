#include "Cop.h"

Cop::Cop() : Enemy(TYPE::TYPE_COP)
{
	setPos('x', rand() % 80 + 0);
	setPos('y', rand() % 24 + 0);
	CharColour = 0x1F;
}

Cop::~Cop()
{
	//nothing
}

void Cop::move()
{
	//overload func, nothing
}

WORD Cop::getCharColour()
{
	return CharColour;
}