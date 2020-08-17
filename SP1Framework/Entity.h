#pragma once
#include <windows.h>
class Entity
{
protected:
	enum TYPE
	{
		TYPE_PLAYER,
		TYPE_KAREN,
		TYPE_CUSTOMER,
		TYPE_COP,
		TYPE_CHAD,
		TYPE_HORDER,
		TYPE_ITEM,
		TOTAL
	};
	TYPE type;
	COORD pos;
public:
	Entity(TYPE t);
	~Entity(void);
	int getPos(char p);
	void setPos(char p, int i);
	virtual void move(void) = 0;
};

