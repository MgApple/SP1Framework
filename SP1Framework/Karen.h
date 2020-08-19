#pragma once
#include "Entity.h"
#include "game.h"
class Karen :public Entity
{
	bool aggrocheck;
public:
	Karen();
	~Karen();
	bool aggro();
	void move(void);
};

