#pragma once
#include "Item.h"
class Soap :public Item
{
	int timer;
public:
	Soap();
	~Soap();
	void timesUp(Map& map, int x, int y);
	void soaptile(Map &map, int x, int y);
};

