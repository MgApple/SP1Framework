#pragma once
#include "Item.h"
#include "Game.h"

class Pass : public Item
{
public:
	Pass();
	~Pass();
	void passtile(Map& map, int x, int y);
};

