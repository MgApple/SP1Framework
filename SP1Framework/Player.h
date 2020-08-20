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
	SKeyEvent* key;
	DIRECTION direction;
public:
	Player();
	~Player();
	void setActive(bool);
	bool getActive();
	void setKey(SKeyEvent*);
	void move(Map &map);
	void render();
	enum DIRECTION getDirection();
};

