#pragma once
class Map
{
private:
	char map[24][80];
public:
	Map(void);
	~Map(void);
	int getEntity(int x, int y);
	void setEntity(int x, int y, int e);
	void loadMap(void);
};