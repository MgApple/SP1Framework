#pragma once
#include <windows.h>
class Entity
{
protected:
	enum TYPE
	{
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_KAREN,
		TYPE_CUSTOMER,
		TYPE_COP,
		TYPE_CHAD,
		TYPE_HORDER,
		TYPE_ITEM,
		TYPE_ENEMY,
		TOTAL
	};
	TYPE type;
	COORD pos;
	bool isSliding, isAttracted;
public:
	Entity(TYPE t);
	~Entity(void);
	int getPos(char p);
	void setPos(char p, int i);
	void setState(char att, bool state);
	virtual void move(void) = 0;
};

