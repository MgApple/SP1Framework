#pragma once
#include "Entity.h"
class Item : public Entity
{
protected:
	enum ITEM
	{
		Drink,
		Soap,
		TP,
		Pass,
		Clock,
		Count
	};
	ITEM type;
public:
	Item(ITEM i);
	~Item(void);
};

