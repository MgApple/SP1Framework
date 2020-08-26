#include "Karen.h"

Karen::Karen() : Enemy(TYPE::TYPE_KAREN)
{
	setPos('x', rand() % 79 + 1);
	setPos('y', rand() % 23 + 1);
	charColor = 0x04;
	aggrocheck = false;
}

Karen::~Karen()
{
	//empty for now
}

bool Karen::aggro(Entity* player,Map &map)
{//needa figure this out
	if ((pow(player->getPos('x') - getPos('x'),2) + (player->getPos('y') - getPos('y')) ^ 2) < 6)
	{
		int playerx = player->getPos('x');
		int playery = player->getPos('y');
		int karenx = getPos('x');
		int kareny = getPos('y');
		int check = (player->getPos('x') - getPos('x')) ^ 2 + (player->getPos('y') - getPos('y')) ^ 2;
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
			if (map.getEntity(x, y) == 'w')
			{
				aggrocheck = false;
				break;
			}
		}
	}
	return aggrocheck;
}

//void useVisionLine(int y1, int x1, int y2, int x2)
//{
//    int i;               // loop counter
//    int ystep, xstep;    // the step on y and x axis
//    int error;           // the error accumulated during the increment
//    int errorprev;       // *vision the previous value of the error variable
//    int y = y1, x = x1;  // the line points
//    int ddy, ddx;        // compulsory variables: the double values of dy and dx
//    int dx = x2 - x1;
//    int dy = y2 - y1;
//    POINT(y1, x1);  // first point
//    // NB the last point can't be here, because of its previous point (which has to be verified)
//    if (dy < 0) {
//        ystep = -1;
//        dy = -dy;
//    }
//    else
//        ystep = 1;
//    if (dx < 0) {
//        xstep = -1;
//        dx = -dx;
//    }
//    else
//        xstep = 1;
//    ddy = 2 * dy;  // work with double values for full precision
//    ddx = 2 * dx;
//    if (ddx >= ddy) {  // first octant (0 <= slope <= 1)
//      // compulsory initialization (even for errorprev, needed when dx==dy)
//        errorprev = error = dx;  // start in the middle of the square
//        for (i = 0; i < dx; i++) {  // do not use the first point (already done)
//            x += xstep;
//            error += ddy;
//            if (error > ddx) {  // increment y if AFTER the middle ( > )
//                y += ystep;
//                error -= ddx;
//                // three cases (octant == right->right-top for directions below):
//                if (error + errorprev < ddx)  // bottom square also
//                    POINT(y - ystep, x);
//                else if (error + errorprev > ddx)  // left square also
//                    POINT(y, x - xstep);
//                else {  // corner: bottom and left squares also
//                    POINT(y - ystep, x);
//                    POINT(y, x - xstep);
//                }
//            }
//            POINT(y, x);
//            errorprev = error;
//        }
//    }
//    else {  // the same as above
//        errorprev = error = dy;
//        for (i = 0; i < dy; i++) {
//            y += ystep;
//            error += ddx;
//            if (error > ddy) {
//                x += xstep;
//                error -= ddy;
//                if (error + errorprev < ddy)
//                    POINT(y, x - xstep);
//                else if (error + errorprev > ddy)
//                    POINT(y - ystep, x);
//                else {
//                    POINT(y, x - xstep);
//                    POINT(y - ystep, x);
//                }
//            }
//            POINT(y, x);
//            errorprev = error;
//        }
//    }
//    // assert ((y == y2) && (x == x2));  // the last point (y2,x2) has to be the same with the last point of the algorithm
//}


void Karen::move(Map &map, const double dt)
{
	elapsedTime += dt;

	if (elapsedTime > moveTime)
	{
		if (aggro(getTarget(), map) == true)
		{
			//does this help?
		}
		else
		{
			Enemy::move(map, dt);
		}
		elapsedTime = 0.0;
	}
}


