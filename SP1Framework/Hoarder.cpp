#include "Hoarder.h"

Hoarder::Hoarder() : Enemy(TYPE::TYPE_HOARDER)
{
    pos.X = 78;
    pos.Y = 12;
    charColor = 0x06;
}

Hoarder::~Hoarder()
{
    //
}

void Hoarder::createPath(Map& map)
{
    // to create an array of the total number of nodes
    nodes = new Node[consoleWidth * consoleHeight];
    for (int x = 0; x < consoleWidth; ++x)
    {
        for (int y = 0; y < consoleHeight; ++y) 
        {
            int i = y * consoleWidth + x;
            nodes[i].pos.X = x; // to find which n6ode
            nodes[i].pos.Y = y;
            // if it's a wall, set bObstacle to true
            if (map.getEntity(y - 1, x) == 'w')
                nodes[i].bObstacle = true;
            else
                nodes[i].bObstacle = false;
            nodes[i].parent = NULL;
            nodes[i].child = NULL;
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

bool Hoarder::checkCollision()
{
    return false;
}

bool Hoarder::solveAStar(Map& map)
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
            nodes[i].child = NULL;
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

void Hoarder::move(Map& map)
{
    if (end->parent != NULL)
    {
        Node* ptr = end->parent;
        pos = ptr->pos;
        // set next node to this node's parent
        end = end->parent;
    }
}
