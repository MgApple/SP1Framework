#include "Hoarder.h"

Hoarder::Hoarder() : Enemy(TYPE::TYPE_HOARDER)
{
    pos.X = 74;
    pos.Y = 12;
    charColor = 0x06;
    //moveTime = 0.2;
    // supposed to be location of items
}

Hoarder::~Hoarder()
{
    //
    delete nodes;
}

void Hoarder::createPath(Map& map)
{
    nodes = new Node[mapWidth * mapHeight];
    // to create an array of the total number of nodes
    for (int x = 0; x < mapWidth; ++x)
    {
        for (int y = 0; y < mapHeight; ++y) 
        {
            int i = y * mapWidth + x;
            nodes[i].pos.X = x; // to find which node
            nodes[i].pos.Y = y;
            // if it's a wall, set bObstacle to true
            if (map.getEntity(x, y - 1) == 'w' ||
                map.getEntity(x,y-1) == 'C' ||
                map.getEntity(x, y - 1) == (char)4 ||
                map.getEntity(x, y - 1) == 'P' ||
                map.getEntity(x, y - 1) == 'K' )
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
}

void Hoarder::updatePath(Map& map)
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
                map.getEntity(x, y - 1) == 'K')
                nodes[i].bObstacle = true;
            else
                nodes[i].bObstacle = false;
            nodes[i].parent = NULL;
            nodes[i].bVisited = false;
        }
    }
}

bool Hoarder::checkCollision()
{
    return false;
}

bool Hoarder::solveAStar()
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

void Hoarder::movement(Map& map, const double dt)
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
            map.setEntity(pos.X, pos.Y - 1, 'H');
        }
        else
        {
            move(map, dt);
        }
        elapsedTime = 0.0;
    }
}

void Hoarder::setStart(int x, int y)
{
    start = &nodes[y * mapWidth + x];
}
