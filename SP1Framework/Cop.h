#pragma once
#include "Enemy.h"
class Cop :
    public Enemy
{
private:
    WORD CharColour;
public:
    Cop();
    ~Cop();
    WORD getCharColour();
    void move();
};

