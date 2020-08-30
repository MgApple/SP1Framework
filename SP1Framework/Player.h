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
	bool hasSpeedBuff;
	DIRECTION direction;

public:
	Player();
	~Player();
	void setSpeed(bool);
	bool getSpeed();
	void move(Map& map, const double dt);
	void movement(Map &map, SKeyEvent* key);
	enum DIRECTION getDirection();
};

