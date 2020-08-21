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
	bool speedBuff;
	bool securityPass;
	bool alarmClock;
	bool isActive;
	DIRECTION direction;
public:
	Player();
	~Player();
	void setActive(bool);
	bool getActive();
	void setSpeed(bool);
	bool getSpeed();
	void move(Map& map);
	void movement(Map &map, SKeyEvent* key);
	void render();
	enum DIRECTION getDirection();
};

