#include "Soap.h"

Soap::Soap() : Item(Item::Soap)
{
}

Soap::~Soap()
{
}

void Soap::timesUp(Map &map, int x, int y)
{
	map.setEntity(x, y, ' ');
}

void Soap::soaptile(Map &map, int x, int y)
{
	map.setEntity(x, y, 'S');
}
