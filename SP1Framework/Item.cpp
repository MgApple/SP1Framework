#include "Item.h"

Item::Item() : Entity(TYPE::TYPE_PLAYER)
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

Item::~Item()
{
	//blank for now
}
