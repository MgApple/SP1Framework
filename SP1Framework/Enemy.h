#pragma once
#include "Entity.h"
class Enemy :public Entity
{
public:
	Enemy(TYPE t);
	~Enemy();
	bool collisionCheck(int intendedx,int intendedy,Map &gamemap);
	virtual void move(Map &map);
};

