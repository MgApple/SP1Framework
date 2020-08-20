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
	setPos('x', rand() % 80);
	setPos('y', rand() % 25);
	charColour = 0x0f;
	//}
}

Customer::~Customer()
{
	//blank for now
}

/*void Customer::move()
{
	//honestly not sure what to do here, just copied off karen's randomisation
	int check = rand() % 4;
	int movement = rand() % 5;
	for (int i = 0; i < movement; i++)
	{
		if (check == 0 && pos.X != 0)
			pos.X--;
		else if (check == 1) //&& pos.X != 0)
			pos.X++;
		else if (check == 2 && pos.Y != 0)
			pos.Y--;
		else if (check == 3)// && pos.Y != 0)
			pos.Y++;
	}
}*/

WORD Customer::getCharColour()
{
	return charColour;
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