#pragma once
#include "Entity.h"
#include "Player.h"

class Chad : public Entity
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