#pragma once
#include "Enemy.h"
class Cop :
    public Enemy
{
private:
    Entity* target;
public:
    Cop();
    ~Cop();
    void move();
    void setPlayer(Entity*);
    bool checkCollision();
};

