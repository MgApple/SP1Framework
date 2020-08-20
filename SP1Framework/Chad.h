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
	//void move(Map &map);
	void setPlayer(Entity*);
	bool checkCollision();
};