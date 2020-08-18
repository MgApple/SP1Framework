#include "Chad.h"

Chad::Chad() : Entity(TYPE::TYPE_CHAD)
{
	/*int x;
	int y;
	while (true)
	{
		x = rand() % 80;
		y = rand() % 25;
		if ()*/
	setPos('x', rand() % 80);
	setPos('y', rand() % 25);
	//}
	charColor = 0x0C;
}

Chad::~Chad()
{
	//blank for now
}

void Chad::move()
{
	if (pos.X < 20)
		++pos.X;
	if (pos.X > 50)
		--pos.X;

}

WORD Chad::getCharColor()
{
	return charColor;
}

void Chad::setPlayer(Player* target)
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
