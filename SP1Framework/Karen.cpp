#include "Karen.h"

Karen::Karen() : Entity(TYPE::TYPE_KAREN)
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
	aggrocheck = false;
}

Karen::~Karen()
{
	//empty for now
}

bool Karen::aggro()
{
	//if (insert check for player here with pythagoras theorem or smth)
	{
		aggrocheck = true;
	}
	return aggrocheck;
}

void Karen::move(void)
{
	if (aggrocheck)
	{
		//insert code for moving to players location by checking 
	}
	else
	{
		//insert code for moving randomly cuz im unsure how to interact with map
		//or whether i am meant to do tht 
		int check = rand() % 4;
		int movement = rand() % 5;
		for (int i = 0; i < movement; i++)
		{
			if (check == 0 && pos.X!=0)
				pos.X--;
			else if (check == 1) //&& pos.X != 0)
				pos.X++;
			else if (check == 2 && pos.Y != 0)
				pos.Y--;
			else if (check==3)// && pos.Y != 0)
				pos.Y++;
		}
	}
}


