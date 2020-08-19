#include "Cop.h"

Cop::Cop() : Enemy(TYPE::TYPE_COP)
{
	setPos('x', rand() % 80);
	setPos('y', rand() % 25);
}

Cop::~Cop()
{
	//nothing
}

void Cop::move()
{
	//overload func, nothing
}