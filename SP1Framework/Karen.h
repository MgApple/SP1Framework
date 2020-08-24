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
	bool aggro(Entity* player,Map &map);
	void move(Map& map, const double dt);
};

