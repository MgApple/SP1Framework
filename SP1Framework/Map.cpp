#include "Map.h"
#include <fstream>
#include <string>

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

//void Map::loadMap(void)
//{
//	std::fstream mapFile;
//	std::string line;
//	mapFile.open("map.txt");
//	if (mapFile) {
//		while (getline(mapFile, line)) {
//			
//		}
//	}
//	else {
//		std::ofstream newMap("map.txt");
//		newMap << 
//	}
//}