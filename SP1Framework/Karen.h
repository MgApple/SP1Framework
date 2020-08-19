#pragma once
#include "Entity.h"
#include "game.h"
class Karen :public Entity
{
	bool aggrocheck;
	WORD charcolour;
public:
	Karen();
	~Karen();
	WORD getCharColour();
	bool aggro();
	void move(void);
};

