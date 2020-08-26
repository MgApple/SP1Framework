#pragma once
#include <Windows.h>
#include "Map.h"
class Item
{
private:
	enum ITEM
	{
		TP = 1, //main item to continue playing
		Soap, //change tiles into soap tiles
		Soda, //no run cooldown
		Pass, //pass by mall cop
		Clock, //alert npcs
		fakeTP, //distract npcs
		TOTAL
	};
	ITEM type;
	COORD pos;
	WORD charColor;
public:
	Item(void);
	Item(int i);
	~Item(void);
	int getItemType(void);
	int getPos(char p);
	WORD getCharColor(void);
	void removeItem(Map& map);
	void move(Map& map, const double dt = 0);
	void reLoc(void);
};