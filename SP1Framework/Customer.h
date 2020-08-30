#pragma once
#include "Enemy.h"
#include "Player.h"

class Customer : public Enemy
{
public:
	Customer();
	~Customer();
	//void move();
	bool checkCollision(Entity*);
};
