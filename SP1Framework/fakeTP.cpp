#include "fakeTP.h"

fakeTP::fakeTP() : Item(Item::fakeTP)
{
	pos.X = rand() % 80 + 0;
	pos.Y = rand() % 24 + 0;
	charColor = 0x0f;
}

fakeTP::~fakeTP(void)
{
	//blank for now
}

void fakeTP::faketile(Map& map, int x, int y)
{
	map.setEntity(x, y, '0');
}