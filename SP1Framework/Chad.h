#pragma once
#include "Enemy.h"
#include "Player.h"

class Chad : public Enemy
{
private:
	Entity* target;
	bool isEnd;

public:
	Chad();
	~Chad();
	void setPlayer(Entity*);
	bool checkCollision();
};