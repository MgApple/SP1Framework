#include "Item.h"
#include <cstdlib>

Item::Item(void)
{
	int i = rand() % 5 + 1;
	switch (i)
	{
	case 1:
		type = Soap;
		isUsable = true;
		icon = (char)22;
		break;
	case 2:
		type = Soda;
		isUsable = true;
		icon = (char)43;
		break;
	case 3:
		type = Pass;
		isUsable = false;
		icon = (char)127;
		break;
	case 4:
		type = Clock;
		isUsable = true;
		icon = (char)13;
		break;
	case 5:
		type = fakeTP;
		isUsable = true;
		icon = (char)7;
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
		isUsable = false;
		icon = (char)8;
		break;
	case 2:
		type = Soap;
		isUsable = true;
		icon = (char)22;
		break;
	case 3:
		type = Soda;
		isUsable = true;
		icon = (char)43;
		break;
	case 4:
		type = Pass;
		isUsable = false;
		icon = (char)127;
		break;
	case 5:
		type = Clock;
		isUsable = true;
		icon = (char)13;
		break;
	case 6:
		type = fakeTP;
		isUsable = true;
		icon = (char)7;
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

char Item::getIcon(void)
{
	return icon;
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