#pragma once
#include "Item.h"
#include "Entity.h"

class fakeTP : public Item
{
public:
	fakeTP();
	~fakeTP();
	void faketile(Map& map, int x, int y);
};

