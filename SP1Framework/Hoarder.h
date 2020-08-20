#pragma once
#include "Enemy.h"

class Hoarder : public Enemy
{
public:
	Hoarder();
	~Hoarder();
	void setPlayer(Entity*);
	bool checkCollision();
};

