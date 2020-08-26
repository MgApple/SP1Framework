#pragma once
#include <windows.h>
#include "Map.h"
class Entity
{
public:
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
	Entity(void);
	Entity(TYPE t);
	~Entity(void);
	int getType(void);
	int getPos(char p);
	void setPos(char p, int i);
	bool getState(char att);
	void setState(char att, bool state);
	void reLoc(void);

	virtual void move(Map& map, const double dt = 0) = 0; // pure virtual won't allow usage of Entity as an object
	virtual WORD getCharColor();
	virtual void setCharColor(WORD);
protected:
	TYPE type;
	COORD pos;
	bool isSliding, isAttracted, isHoldingTP;
	WORD charColor;

};

