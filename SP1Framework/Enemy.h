#pragma once
#include "Entity.h"
class Enemy :public Entity
{
public:
	Enemy(TYPE t);
	~Enemy();
	virtual void move(void);
};

