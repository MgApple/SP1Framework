#pragma once
#include "Enemy.h"
#include "Player.h"

class Chad : public Enemy
{
private:
	WORD charColor;
	Entity* target;
	bool isEnd;

public:
	Chad();
	~Chad();
	//void move(Map &map);
	WORD getCharColor();
	void setPlayer(Entity*);
	bool checkCollision();
};