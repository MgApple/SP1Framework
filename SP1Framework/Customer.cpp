#include "Customer.h"

Customer::Customer() : Enemy(TYPE::TYPE_CUSTOMER)
{
	/*int x;
	int y;
	while (true)
	{
		x = rand() % 80;
		y = rand() % 25;
		if ()*/
	setPos('x', rand() % 79 + 1);
	setPos('y', rand() % 23 + 1);
	charColor = 0x0f;
	//}
}

Customer::~Customer()
{
	//blank for now
}

void Customer::setPlayer(Entity* target)
{
	this->target = target;
}

bool Customer::checkCollision()
{
	COORD targetPos;
	targetPos.X = target->getPos('x');
	targetPos.Y = target->getPos('y');

	if (targetPos.X == pos.X && targetPos.Y == pos.Y)
		return true;
	else
		return false;
}