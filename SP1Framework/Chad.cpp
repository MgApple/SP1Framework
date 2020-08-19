#include "Chad.h"

Chad::Chad() : Enemy (TYPE::TYPE_CHAD)
{
	/*int x;
	int y;
	while (true)
	{
		x = rand() % 80;
		y = rand() % 25;
		if ()*/
	setPos('x', rand() % 80 + 0);
	setPos('y', rand() % 24 + 0);
	//}
	charColor = 0x0C;
	isEnd = false;
}

Chad::~Chad()
{
	//blank for now
}

void Chad::move(Map &map)
{
	// to be changed
	if(isEnd == false)
		++pos.X;
	if (pos.X > 50)
		isEnd = true;
	if (isEnd == true)
		--pos.X;
	if (pos.X < 20)
		isEnd = false;
}

WORD Chad::getCharColor()
{
	return charColor;
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

