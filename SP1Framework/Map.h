#pragma once
class Map
{
private:
	char map[24][80];
	char tutorial[24][50];
public:
	Map(void);
	~Map(void);
	int getEntity(int x, int y);
	void setEntity(int x, int y, int e);
	void loadMap(void);
	void loadtutorial(void);
};