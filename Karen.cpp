#include "Karen.h"

Karen::Karen() : Enemy(TYPE::TYPE_KAREN)
{
	/*int x;
	int y;
	while (true)
	{
		x = rand() % 80;
		y = rand() % 25;
		if ()*/
	setPos('x', rand() % 79 + 1);
	setPos('y', rand() % 23 + 1);
	//}
	aggrocheck = false;
}

Karen::~Karen()
{
	//empty for now
}

bool Karen::aggro(Entity* player,Map &map)
{
	if (((player->getPos('x') - getPos('x')) ^ 2 + (player->getPos('y') - getPos('y')) ^ 2) < 6)
	{
		int x1 = getPos('x');
		int x2 = player->getPos('x');
		int y1 = getPos('y');
		int y2 = player->getPos('y');
		int len;
		//code to check whether wall is blocking LOS
		if (abs(x2 - x1) > abs(y2 - y1))
			len = abs(x2 - x1);
		else
			len = abs(y2 - y1);
		for (int i = 0; i < len; i++)
		{
			//interpolate between(x1, y1) and (x2, y2)
			float t = float(i) / len;
			// at t = 0.0 we get(x1, y1); at t = 1.0 we get(x2, y2)
			int x = round(x1 * (1.0 - t) + (x2 * t));
			int y = round(y1 * (1.0 - t) + (y2 * t));
			if (map.getEntity(x,y)=='w')
			{
				aggrocheck = false;
				break;
			}
		}
	}
	return aggrocheck;
}

void Karen::createPath(Map& map)
{
	// to create an array of the total number of nodes
	nodes = new Node[consoleWidth * consoleHeight];
	for (int x = 0; x < consoleWidth; ++x)
	{
		for (int y = 0; y < consoleHeight; ++y)
		{
			int i = y * consoleWidth + x;
			nodes[i].pos.X = x; // to find which node
			nodes[i].pos.Y = y;
			// if it's a wall, set bObstacle to true
			if (map.getEntity(x, y - 1) == 'w')
				nodes[i].bObstacle = true;
			else
				nodes[i].bObstacle = false;
			nodes[i].parent = NULL;
			nodes[i].bVisited = false;
		}
	}

	// to create connections between the nodes
	for (int x = 0; x < consoleWidth; ++x)
	{
		// y = 1 because the top row is not included
		for (int y = 0; y < consoleHeight; ++y)
		{
			int i = y * consoleWidth + x;
			if (y > 1 && nodes[(y - 1) * consoleWidth + (x + 0)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y - 1) * consoleWidth + (x + 0)]);
			if (y < consoleHeight - 1 && nodes[(y + 1) * consoleWidth + (x + 0)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y + 1) * consoleWidth + (x + 0)]);
			if (x > 1 && nodes[(y + 0) * consoleWidth + (x - 1)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y + 0) * consoleWidth + (x - 1)]);
			if (x < consoleWidth - 1 && nodes[(y + 0) * consoleWidth + (x + 1)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y + 0) * consoleWidth + (x + 1)]);
		}
	}

	end = &nodes[pos.Y * consoleWidth + pos.X];
	// supposed to be location of items
	start = &nodes[(consoleHeight / 2) * (consoleWidth / 2) + (consoleWidth / 2)];
}

bool Karen::solveAStar(Map& map)
{
	// reset navigation graph - default all node states
	for (int x = 0; x < consoleWidth; ++x)
	{
		for (int y = 0; y < consoleHeight; ++y)
		{
			int i = y * consoleWidth + x;
			nodes[i].bVisited = false;
			nodes[i].globalGoal = INFINITY;
			nodes[i].localGoal = INFINITY;
			nodes[i].parent = NULL;
		}
	}

	// pythagoras' theorem - sqrt((x1-x2)^2 + (y1-y2)^2)
	auto distance = [](Node* a, Node* b)
	{
		return sqrtf((float)((a->pos.X - b->pos.X) * (a->pos.X - b->pos.X) + (a->pos.Y - b->pos.Y) * (a->pos.Y - b->pos.Y)));
	};

	auto heuristic = [distance](Node* a, Node* b)
	{
		return distance(a, b);
	};

	// start conditions
	Node* current = start;
	start->localGoal = 0.0f;
	start->globalGoal = heuristic(start, end);

	std::list<Node*> notTestedNodes;
	notTestedNodes.push_back(start);

	while (!notTestedNodes.empty())
	{
		// sort untested nodes by global goal, lowest first
		notTestedNodes.sort([](const Node* lhs, const Node* rhs)
			{
				return lhs->globalGoal < rhs->globalGoal;
			});

		// front of list potentially be the lowest distance
		while (!notTestedNodes.empty() && notTestedNodes.front()->bVisited)
			notTestedNodes.pop_front();

		// if no valid nodes left to test, break
		if (notTestedNodes.empty())
			break;

		current = notTestedNodes.front();
		current->bVisited = true; // go through nodes only once

		// check node's neighbours
		for (auto nodeNeighbour : current->neighbours)
		{
			// only if neighbour not visited and is not an obstacle
			// add to the list
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == false)
				notTestedNodes.push_back(nodeNeighbour);

			float possiblyLowerGoal = current->localGoal + distance(current, nodeNeighbour);

			// FIX LOCAL GOAL AND GLOBAL GOAL NOT UPDATING
			if (possiblyLowerGoal < nodeNeighbour->localGoal)
			{
				nodeNeighbour->parent = current;
				nodeNeighbour->localGoal = possiblyLowerGoal;
				nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, end);
			}
		}
	}
	return true;
}

void Karen::movement(Map &map, const double dt)
{
	elapsedTime += dt;

	if (elapsedTime > moveTime)
	{
		if (aggro(getTarget(), map) == true)
		{
			Node* ptr = end->parent;
			pos = ptr->pos;
			// set next node to this node's parent
			end = end->parent;
		}
		else
		{
			move(map, dt);
		}
		elapsedTime = 0.0;
	}
}


