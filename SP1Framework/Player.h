#pragma once
#include "Entity.h"
#include "game.h"

class Player : public Entity
{
private:
	enum DIRECTION
	{
		UP,
		LEFT,
		DOWN,
		RIGHT,
		NONE
	};
	int inventory[3];
	DIRECTION direction;

public:
	Player();
	~Player();
	void move(Map& map, const double dt);
	void movement(Map &map, SKeyEvent* key);
	enum DIRECTION getDirection();
};

