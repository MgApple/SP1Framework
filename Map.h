#pragma once
class Map
{
private:
	int map[80][25];
public:
	Map(void);
	~Map(void);
	int getEntity(char x, char y);
	void setEntity(char x, char y, int e);
};