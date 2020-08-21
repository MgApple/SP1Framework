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
		fakeTP, //distract npcs
		goal
	};
	ITEM type;
public:
	Item(ITEM i);
	~Item(void);
};

