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
		//code to check for wall is blocking LOS
		/*if (abs(x2 - x1) > abs(y2 - y1))
			len = abs(x2 - x1);
		else
			len = abs(y2 - y1);*/
			/*
				for (int i=0;i<len;i++):
			# interpolate between(x1, y1) and (x2, y2)
				t = float(i) / len
				# at t = 0.0 we get(x1, y1); at t = 1.0 we get(x2, y2)
				x = round(x1 * (1.0 - t) + x2 * t)
				y = round(y1 * (1.0 - t) + y2 * t)
				# now check tile(x, y)*/
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


