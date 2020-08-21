#include "Pass.h"

Pass::Pass() : Item(Item::Pass)
{
	pos.X = rand() % 80 + 0;
	pos.Y = rand() % 24 + 0;
	charColor = 0x0f;
}

Pass::~Pass(void)
{
	//blank for now
}

void Pass::passtile(Map& map, int x, int y)
{
	map.setEntity(x, y, '[]');
}