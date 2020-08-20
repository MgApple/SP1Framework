#include "Hoarder.h"

Hoarder::Hoarder() : Enemy(TYPE::TYPE_HOARDER)
{
    setPos('x', 15);
    setPos('y', 16);
    charColor = 0x06;
}

Hoarder::~Hoarder()
{
	//
}

void Hoarder::move(Map& map)
{
    COORD c;
    map.loadMap();
    for (int R = 0; R < 24; R++)
    {
        c.Y = R + 1;
        for (int C = 0; C < 80; C++)
        {
            c.X = C;
            if (map.getEntity(R, C) == 'w')
                setPos('x', getPos('x') + 1);
        }
    }
}
