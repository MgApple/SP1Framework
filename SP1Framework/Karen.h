#pragma once
#include "Enemy.h"
#include <vector>
#include <list>
#include <math.h>

class Karen :public Enemy
{
private:
	//bool aggrocheck;
	struct Node
	{
		bool bObstacle = false;
		bool bVisited = false;
		float globalGoal = INFINITY; // INFINITY is a float type
		float localGoal = INFINITY;
		COORD pos = { 0, 0 };
		std::vector<Node*> neighbours;
		Node* parent = NULL;
	};

	int xcheck, ycheck;
	int mapWidth = 80;
	int mapHeight = 25;
	bool isEnd;
	Node* nodes = NULL;
	Node* start = NULL;
	Node* end = NULL;
public:
	Karen();
	~Karen();
	bool aggro(Entity* player,Map &map);
	void createPath(Map& map);
	void updatePath(Map& map);
	bool solveAStar();
	void setStart(Map &map);
	void setIsEnd(bool isend);
	bool getIsEnd();
	void move(Map& map, const double dt);
	bool checkCollision(Entity*);
};

