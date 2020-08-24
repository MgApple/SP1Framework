#pragma once
#include "Entity.h"
class Enemy :public Entity
{
	char collidingCheck[6] = { 'w','C',(char)4,'P','H','K'};
	Entity* target;

protected:
	double elapsedTime;
	double moveTime;

public:
	Enemy(TYPE t);
	~Enemy();
	void setTarget(Entity* target);
	Entity* getTarget();
	bool collisionCheck(int intendedx,int intendedy,Map &gamemap);
	virtual void move(Map &map, const double dt = 0);

};

