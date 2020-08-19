#pragma once
#include "Entity.h"
class Enemy :public Entity
{
	char collidingCheck[6] = {' ','T','A','D','S','P'};
public:
	Enemy(TYPE t);
	~Enemy();
	bool collisionCheck(int intendedx,int intendedy,Map &gamemap);
	virtual void move(Map &map);
};

