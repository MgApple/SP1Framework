#include "Hoarder.h"

Hoarder::Hoarder() : Enemy(TYPE::TYPE_HOARDER)
{
    setPos('x', 15);
    setPos('y', 16);
    charColor = 0x06;
}

Hoarder::~Hoarder()
{
	//
}

void Hoarder::setPlayer(Entity*)
{
}

bool Hoarder::checkCollision()
{
    return false;
}