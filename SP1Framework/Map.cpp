#include "Map.h"

Map::Map(void) : map{ 0 }
{
	// empty
}

Map::~Map(void)
{
	// empty
}

int Map::getEntity(char x, char y)
{
	return map[x][y];
}

void Map::setEntity(char x, char y, int e)
{
	map[x][y] = e;
}
