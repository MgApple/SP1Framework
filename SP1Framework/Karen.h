#pragma once
#include "Enemy.h"
#include "game.h"
class Karen :public Enemy
{
private:
	bool aggrocheck;
	int LRcheck();
	int UDcheck();
public:
	Karen();
	~Karen();
	bool aggro();
	void move(Map &map);
};

