#pragma once
class Map
{
private:
	char map[80][24];
	char tutorial[24][50];
public:
	Map(void);
	~Map(void);
	char getEntity(int x, int y);
	void setEntity(int x, int y, char e);
	void reloadMap(void);
	void loadMap(void);
	void loadtutorial(void);
};