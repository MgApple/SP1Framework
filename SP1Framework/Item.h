#pragma once
#include "Entity.h"
class Item : public Entity
{
protected:
	enum ITEM
	{
		TP, //main item to continue playing
		Soap, //change tiles into soap tiles
		Soda, //no run cooldown
		Pass, //pass by mall cop
		Clock, //alert npcs
		fakeTP //distract npcs
	};
	ITEM type;
	char character;
public:
	Item(ITEM i);
	~Item(void);
	void setTile(Map& map, int x, int y);
};

