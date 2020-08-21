#pragma once
#include "Entity.h"
#include "Player.h"
class Item : public Entity
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
public:
	Item(ITEM i);
	~Item(void);
	void removeItem(Map& map);
	void pickedUp(Map& map, Entity *entity, Player &player);
	void move(Map& map);
};