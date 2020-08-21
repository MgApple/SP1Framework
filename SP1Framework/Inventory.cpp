#include "Inventory.h"

Inventory::Inventory():inventoryArray{' '}
{
}

Inventory::~Inventory()
{
	//blank
}

void Inventory::pickedUpItem(int itemSlot, char item)
{
	if (itemSlot < 4 && itemSlot>0)
		inventoryArray[itemSlot-1] = item;
}

char Inventory::useItem(int itemSlot)
{
	char temp = inventoryArray[itemSlot - 1];
	inventoryArray[itemSlot - 1] = ' ';
	return temp;
}

char Inventory::getItem(int itemSlot)
{
	return inventoryArray[itemSlot-1];
}
