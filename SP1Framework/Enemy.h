#pragma once
#include "Entity.h"
class Enemy :public Entity
{
	char collidingCheck[6] = { 'w','T','A','D','S','P'};
	Entity* target;
public:
	Enemy(TYPE t);
	~Enemy();
	void setTarget(Entity* target);
	Entity* getTarget();
	bool collisionCheck(int intendedx,int intendedy,Map &gamemap);
	virtual void move(Map &map);

};

