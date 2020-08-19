#include "Customer.h"

Customer::Customer() : Entity(TYPE::TYPE_CUSTOMER)
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
}

Customer::~Customer()
{
	//blank for now
}

void Customer::move()
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
}