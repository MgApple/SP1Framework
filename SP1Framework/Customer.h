#pragma once
#include "Enemy.h"
#include "game.h"

class Customer : public Enemy
{
private:
	Entity* target;
public:
	Customer();
	~Customer();
	//void move();
	void setPlayer(Entity*);
	bool checkCollision();
};
