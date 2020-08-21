#pragma once
class Inventory
{
	char inventoryArray[3];
public:
	Inventory();
	~Inventory();
	void pickedUpItem(int itemSlot, char item);
	char useItem(int itemSlot);
	char getItem(int itemSlot);
};

