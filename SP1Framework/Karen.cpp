#include "Karen.h"

Karen::Karen() : Enemy(TYPE::TYPE_KAREN)
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

bool Karen::aggro(Entity* player,Map &map)
{
	if (((player->getPos('x') - getPos('x')) ^ 2 + (player->getPos('y') - getPos('y')) ^ 2) < 6)
	{
		int x1 = getPos('x');
		int x2 = player->getPos('x');
		int y1 = getPos('y');
		int y2 = player->getPos('y');
		int len;
		//code to check for wall is blocking LOS
		if (abs(x2 - x1) > abs(y2 - y1))
			len = abs(x2 - x1);
		else
			len = abs(y2 - y1);
		for (int i = 0; i < len; i++)
		{
			//interpolate between(x1, y1) and (x2, y2)
			float t = float(i) / len;
			// at t = 0.0 we get(x1, y1); at t = 1.0 we get(x2, y2)
			/*int x = round(x1 * (1.0 - t) + x2 * t);
			int y = round(y1 * (1.0 - t) + y2 * t);
			if map.getEntity(x,y)=='w'
			{
				aggrocheck = false;
				break;
			}*/
				
		}
	}
	return aggrocheck;
}

void Karen::move(Map &map, const double dt)
{
	if (aggro(getTarget(), map) == true)
	{
		//insert code for moving to players location by checking 
	}
	else
	{
		move(map, dt);
	}
}


