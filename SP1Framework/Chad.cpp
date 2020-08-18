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
}

Chad::~Chad()
{
	//blank for now
}
