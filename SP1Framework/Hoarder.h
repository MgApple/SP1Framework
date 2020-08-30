#pragma once
#include <vector>
#include <list>
#include "Enemy.h"

class Hoarder : public Enemy
{
private:
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

	Node* nodes = NULL;
	int mapWidth = 80;
	int mapHeight = 25; 

	Node* start = NULL;
	Node* end = NULL;

public:
	Hoarder();
	~Hoarder();
	void createPath(Map& map);
	void updatePath(Map& map);
	bool checkCollision();
	bool solveAStar();
	void movement(Map& map, const double dt);
	void setStart(int, int);
};

