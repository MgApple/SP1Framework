#include "Chad.h"

Chad::Chad() : Enemy (TYPE::TYPE_CHAD)
{
	pos.X = rand() % 80 + 0;
	pos.Y = rand() % 24 + 0;
	charColor = 0x0C;
	isEnd = false;
	target = NULL;
}

Chad::~Chad()
{
	//blank for now
}

//void Chad::move(Map &map)
//{
//	// to be changed
//	if(isEnd == false)
//		++pos.X;
//	if (pos.X > 50)
//		isEnd = true;
//	if (isEnd == true)
//		--pos.X;
//	if (pos.X < 20)
//		isEnd = false;
//}

void Chad::setPlayer(Entity* target)
{
	this->target = target;
}

bool Chad::checkCollision()
{
	COORD targetPos;
	targetPos.X = target->getPos('x');
	targetPos.Y = target->getPos('y');

	if ((targetPos.X == pos.X + 1 && targetPos.Y == pos.Y) ||
		(targetPos.X == pos.X - 1 && targetPos.Y == pos.Y) ||
		(targetPos.X == pos.X && targetPos.Y == pos.Y + 1) ||
		(targetPos.X == pos.X && targetPos.Y == pos.Y - 1))
		return true;
	else
		return false;
}

