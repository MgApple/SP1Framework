#include "Karen.h"

Karen::Karen() : Enemy(TYPE::TYPE_KAREN)
{
	pos.X = rand() % 79 + 1;
	pos.Y = rand() % 23 + 1;
	charColor = 0x04;
	xcheck = rand() % 79 + 1;
	ycheck = rand() % 23 + 1;
	isEnd = false;
}

Karen::~Karen()
{
	delete nodes;
}

bool Karen::aggro(Entity* player,Map &map)	//checks whether Karen is in LOS of player
{
	bool aggrocheck = true;
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
		//interpolate between(x1, y1) and (x2, y2) //estimated checks between points
		float t = float(i) / len;
		// at t = 0.0 we get(x1, y1); at t = 1.0 we get(x2, y2)
		int x = round(x1 * (1.0 - t) + (x2 * t));
		int y = round(y1 * (1.0 - t) + (y2 * t));
		if (map.getEntity(x, y - 1) == 'w')
		{
			aggrocheck = false;
			break;
		}
	}
	return aggrocheck;
}

void Karen::createPath(Map& map)
{
	// to create an array of the total number of nodes
	nodes = new Node[mapWidth * mapHeight];
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			int i = y * mapWidth + x;
			nodes[i].pos.X = x; // to find which node
			nodes[i].pos.Y = y;
			// if it's a wall, set bObstacle to true
			if (map.getEntity(x, y - 1) == 'w' ||
				map.getEntity(x, y - 1) == 'C' ||
				map.getEntity(x, y - 1) == (char)4 ||
				map.getEntity(x, y - 1) == 'P' ||
				map.getEntity(x, y - 1) == 'H')
				nodes[i].bObstacle = true;
			else
				nodes[i].bObstacle = false;
			nodes[i].parent = NULL;
			nodes[i].bVisited = false;
		}
	}

	// to create connections between the nodes
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			int i = y * mapWidth + x;
			if (y > 1 && nodes[(y - 1) * mapWidth + (x + 0)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y - 1) * mapWidth + (x + 0)]);
			if (y < mapHeight - 1 && nodes[(y + 1) * mapWidth + (x + 0)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y + 1) * mapWidth + (x + 0)]);
			if (x > 1 && nodes[(y + 0) * mapWidth + (x - 1)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y + 0) * mapWidth + (x - 1)]);
			if (x < mapWidth - 1 && nodes[(y + 0) * mapWidth + (x + 1)].bObstacle == false)
				nodes[i].neighbours.push_back(&nodes[(y + 0) * mapWidth + (x + 1)]);
		}
	}
	// hoarder position
	end = &nodes[pos.Y * mapWidth + pos.X];
	start = &nodes[ycheck * mapWidth + xcheck];
	if (start->bObstacle)
	{
		setStart(map);
		start = &nodes[ycheck * mapWidth + xcheck];
	}
}

void Karen::updatePath(Map& map)
{
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			int i = y * mapWidth + x;
			nodes[i].pos.X = x; // to find which node
			nodes[i].pos.Y = y;
			// if it's a wall, set bObstacle to true
			if (map.getEntity(x, y - 1) == 'w' ||
				map.getEntity(x, y - 1) == 'C' ||
				map.getEntity(x, y - 1) == (char)4 ||
				map.getEntity(x, y - 1) == 'P' ||
				map.getEntity(x, y - 1) == 'H')
				nodes[i].bObstacle = true;
			else
				nodes[i].bObstacle = false;
			nodes[i].parent = NULL;
			nodes[i].bVisited = false;
		}
	}
}

bool Karen::solveAStar()
{
	// reset navigation graph - default all node states
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			int i = y * mapWidth + x;
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

void Karen::move(Map &map, const double dt)
{
	elapsedTime += dt;
	if (elapsedTime > moveTime)
	{
		if (end->parent != NULL) 
		{
			map.setEntity(pos.X, pos.Y - 1, ' ');
			Node* ptr = end->parent;
			pos = ptr->pos;
			// set next node to this node's parent
			end = end->parent;
			map.setEntity(pos.X, pos.Y-1, 'K');
		}
		else
		{
			isEnd = true;
		}
		elapsedTime = 0.0;
	}
}

void Karen::setStart(Map &map)
{
	xcheck = rand() % 79 + 1;
	ycheck = rand() % 23 + 1;
	if (map.getEntity(xcheck, ycheck - 1) != ' ')
	{
		while (map.getEntity(xcheck, ycheck - 1) != ' ')
		{
			xcheck = rand() % 79 + 1;
			ycheck = rand() % 23 + 1;
		}
	}
	start = &nodes[ycheck * mapWidth + xcheck];
}

void Karen::setIsEnd(bool isend)
{
	isEnd = isend;
}

bool Karen::getIsEnd()
{
	return isEnd;
}

bool Karen::checkCollision(Entity* target)
{
	COORD targetPos;
	targetPos.X = target->getPos('x');
	targetPos.Y = target->getPos('y');

	if (targetPos.X == pos.X && targetPos.Y == pos.Y)
		return true;
	else
		return false;
}