#pragma once
#include "Enemy.h"
#include "Player.h"

class Customer : public Enemy
{
private:
	WORD charColour;
	Entity* target;

public:
	Customer();
	~Customer();
	//void move();
	WORD getCharColour();
	void setPlayer(Entity*);
	bool checkCollision();
};
