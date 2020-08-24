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
	int consoleWidth = 80;
	int consoleHeight = 25; // might be 24

	Node* start = NULL;
	Node* end = NULL;

public:
	Hoarder();
	~Hoarder();
	void createPath(Map& map);
	bool checkCollision();
	bool solveAStar(Map& map);
	void movement(Map& map, const double dt);
};

