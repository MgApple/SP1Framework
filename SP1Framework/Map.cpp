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

char Map::getEntity(int x, int y)
{
	return map[x][y];
}

void Map::setEntity(int x, int y, int e)
{
	map[x][y] = e;
}

void Map::loadMap(void)
{
	std::ifstream mapFile;
	std::string line;
	mapFile.open("map.txt");
	if (mapFile) {
		int lineNbr = 0;
		while (getline(mapFile, line)) {
			for (unsigned int i = 0; i < line.length(); i++)
			{
				map[i][lineNbr] = line[i];
			}
			lineNbr++;
		}
	}
}

void Map::loadtutorial(void)
{
	std::ifstream tutorial;
	std::string line;
	tutorial.open("Tutorial_LVL.txt");
	if (tutorial) {
		int lineNbr = 0;
		while (getline(tutorial, line)) {
			for (unsigned int i = 0; i < line.length(); i++)
			{
				map[lineNbr][i] = line[i];
			}
			lineNbr++;
		}
	}
}