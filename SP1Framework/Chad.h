#pragma once
#include "Enemy.h"
#include "Player.h"

class Chad : public Enemy
{
private:
	WORD charColor;
	Player* target;
	bool isEnd;

public:
	Chad();
	~Chad();
	void move();
	WORD getCharColor();
	void setPlayer(Player*);
	bool checkCollision();
};