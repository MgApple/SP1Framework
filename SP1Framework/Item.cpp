#include "Item.h"
#include <cstdlib>

Item::Item(void)
{
	int i = rand() % 5 + 1;
	switch (i)
	{
	case 1:
		type = Soap;
		break;
	case 2:
		type = Soda;
		break;
	case 3:
		type = Pass;
		break;
	case 4:
		type = Clock;
		break;
	case 5:
		type = fakeTP;
		break;
	}
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
	charColor = 0x6f;
}

Item::Item(int i)
{
	switch (i)
	{
	case 1:
		type = TP;
		break;
	case 2:
		type = Soap;
		break;
	case 3:
		type = Soda;
		break;
	case 4:
		type = Pass;
		break;
	case 5:
		type = Clock;
		break;
	case 6:
		type = fakeTP;
		break;
	}
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
	charColor = 0x6f;
}

Item::~Item(void)
{
	//blank for now
}

int Item::getItemType(void)
{
	return type;
}

int Item::getPos(char p)
{
	if (p == 'x')
		return pos.X;
	else
		return pos.Y;
}

WORD Item::getCharColor(void)
{
	return charColor;
}

void Item::removeItem(Map& map)
{
	map.setEntity(pos.X, pos.Y, ' ');
}

void Item::move(Map& map, const double dt)
{
	// empty (override)
}