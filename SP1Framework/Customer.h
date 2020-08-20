#pragma once
#include "Entity.h"
#include "game.h"

class Customer : public Entity
{
public:
	Customer();
	~Customer();
	void move();
	bool checkCollision();
};
