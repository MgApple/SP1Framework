#pragma once
#include "Entity.h"
#include "game.h"

class Player : public Entity
{
private:
	int inventory[3];
	int stamina;
	bool speedBuff;
	bool securityPass;
	bool alarmClock;
public:
	Player();
	~Player();
	void move();
};

