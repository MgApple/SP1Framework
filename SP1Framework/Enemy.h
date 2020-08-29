#pragma once
#include "Entity.h"
class Enemy :public Entity
{
	char collidingCheck[6] = { 'w','C',char(4),'P','H','K'};
	Entity* target;

protected:
	double elapsedTime;
	double moveTime;

public:
	Enemy(TYPE t);
	~Enemy();
	bool collisionCheck(int intendedx,int intendedy,Map &gamemap);
	virtual void move(Map &map, const double dt = 0);

};

