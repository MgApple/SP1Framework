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
	int stamina;
	bool hasSpeedBuff, hasStaminaBuff, hasSecurityPass, isActive;
	DIRECTION direction;

public:
	Player();
	~Player();
	int getInventory(int idx);
	void setInventory(int idx, int item);
	bool getPState(char att);
	void setPState(char att, bool state);
	void setActive(bool);
	bool getActive();
	void setSpeed(bool);
	bool getSpeed();
	void move(Map& map, const double dt);
	void movement(Map &map, SKeyEvent* key);
	void render();
	enum DIRECTION getDirection();
};

