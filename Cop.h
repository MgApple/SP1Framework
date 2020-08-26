#pragma once
#include "Enemy.h"
class Cop :
    public Enemy
{
public:
    Cop();
    ~Cop();
    void move();
};

