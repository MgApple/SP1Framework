#include "Item.h"
#include <cstdlib>

Item::Item(void)
{
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
}

Item::~Item(void)
{
	//blank for now
}

int Item::getPos(char p)
{
	if (p == 'x')
		return pos.X;
	else
		return pos.Y;
}

void Item::setPos(char p, int x)
{
	if (p == 'x')
		pos.X = x;
	else
		pos.Y = x;
}

void Item::removeItem(Map& map)
{
	map.setEntity(pos.X, pos.Y - 1, ' ');
}

void Item::move(Map& map, const double dt)
{
	// empty (override)
}

void Item::reLoc(void)
{
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
}