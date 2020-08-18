#pragma once
#include "Entity.h"
class Item :
	public Entity
{
	enum item
	{
		Drink,
		Soap,
		TP,
		Pass,
		Clock,
		Count
	};
public:
	Item();
	~Item();
};

