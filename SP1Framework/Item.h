#pragma once
#include <Windows.h>
#include "Map.h"
class Item
{
private:
	COORD pos;
public:
	Item(void);
	~Item(void);
	int getPos(char p);
	void setPos(char p, int x);
	void removeItem(Map& map);
	void move(Map& map, const double dt = 0);
	void reLoc(void);
};