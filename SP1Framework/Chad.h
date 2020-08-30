#pragma once
#include "Enemy.h"
#include "Player.h"

class Chad : public Enemy
{
private:
	bool isEnd;

public:
	Chad();
	~Chad();
	bool checkCollision(Entity*);
};