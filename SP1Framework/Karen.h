#pragma once
#include "Enemy.h"
#include <vector>
#include <list>
#include <math.h>

class Karen :public Enemy
{
private:
	bool aggrocheck;
public:
	Karen();
	~Karen();
	bool aggro(Entity* player,Map &map);
	/*void useVisionLine(int y1, int x1, int y2, int x2);*/
	void move(Map& map, const double dt);
};

