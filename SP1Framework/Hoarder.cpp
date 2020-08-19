#include "Hoarder.h"

Hoarder::Hoarder() : Enemy(TYPE::TYPE_HOARDER)
{
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
       /* c.Y = R + 1;
        for (int C = 0; C < 80; C++)
        {
            c.X = C;
            if (map.getEntity(R, C) == 'w')
                g_Console.writeToBuffer(c, (char)219, colors[4]);
            else if (map.getEntity(R, C) == ' ')
                g_Console.writeToBuffer(c, (char)32, colors[12]);
            else
                g_Console.writeToBuffer(c, 'n', colors[12]);
        }*/
    }
}
