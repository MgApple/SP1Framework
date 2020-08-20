#include "Item.h"

Item::Item(ITEM i) : Entity(TYPE::TYPE_ITEM)
{
	character = ' ';
	/*int x;
	int y;
	while (true)
	{
		x = rand() % 80;
		y = rand() % 25;
		if ()*/
	setPos('x', rand() % 79);
	setPos('y', rand() % 24);
	//}
	type = i;
}

Item::~Item()
{
	//blank for now
}

void Item::setTile(Map& map, int x, int y)
{
	map.setEntity(x, y, character);
}
