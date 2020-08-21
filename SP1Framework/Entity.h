#pragma once
#include <windows.h>
#include "Map.h"
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
		TYPE_HOARDER,
		TYPE_ITEM,
		TOTAL
	};
	TYPE type;
	COORD pos;
	bool isSliding, isAttracted, isHoldingTP;
	WORD charColor;

public:
	Entity(void);
	Entity(TYPE t);
	~Entity(void);
	int getType(void);
	int getPos(char p);
	void setPos(char p, int i);
	bool getState(char att);
	void setState(char att, bool state);
	virtual void move(Map &map) = 0;
	virtual WORD getCharColor();
	virtual void setCharColor(WORD);
};

