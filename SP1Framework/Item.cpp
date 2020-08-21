#include "Item.h"

Item::Item(ITEM i) : Entity(TYPE::TYPE_ITEM)
{
	setPos('x', rand() % 80);
	setPos('y', rand() % 24);
	type = i;
}

Item::~Item()
{
	//blank for now
}

void Item::removeItem(Map& map)
{
	map.setEntity(getPos('x'), getPos('y'), ' ');
}

void Item::pickedUp(Map& map, Entity *entity, Player &player)
{
	switch(type) {
	case TP:
		if (!(entity->getState('t'))) { // if is not holding toilet paper
			entity->setState('t', true);
			removeItem(map);
		}
		break;
	case Soap:
		if (entity->getType() == 0) { // if is a player
			for (int i = 0; i < 3; i++)
			{
				if (player.getInventory(i) == 0) { // if the player has an empty inventory slot
					player.setInventory(i, Soap);
					removeItem(map);
				}
			}
		}
		else {
			removeItem(map);
		}
		break;
	case Soda:
		if (entity->getType() == 0) {
			for (int i = 0; i < 3; i++)
			{
				if (player.getInventory(i) == 0) {
					player.setInventory(i, Soda);
					removeItem(map);
				}
			}
		}
		else {
			removeItem(map);
		}
		break;
	case Pass:
		if (entity->getType() == 0) {
			for (int i = 0; i < 3; i++)
			{
				if (player.getInventory(i) == 0) {
					player.setInventory(i, fakeTP);
					player.setPState('p', true);
					removeItem(map);
				}
			}
		}
		else {
			removeItem(map);
		}
		break;
	case Clock:
		if (entity->getType() == 0) {
			for (int i = 0; i < 3; i++)
			{
				if (player.getInventory(i) == 0) {
					player.setInventory(i, Clock);
					removeItem(map);
				}
			}
		}
		else {
			removeItem(map);
		}
		break;
	case fakeTP:
		if (entity->getType() == 0) {
			for (int i = 0; i < 3; i++)
			{
				if (player.getInventory(i) == 0) {
					player.setInventory(i, fakeTP);
					removeItem(map);
				}
			}
		}
		else {
			removeItem(map);
		}
		break;
	}
}

void Item::move(Map& map)
{
	// empty (override)
}