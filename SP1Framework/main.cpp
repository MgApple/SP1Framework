// This is the main file to hold everthing together

#include <time.h>
#include "Framework\timer.h"
#include "game.h"

CStopWatch g_Timer;                            // Timer function to keep track of time and the frame rate
bool g_bQuitGame = false;                    // Set to true if you want to quit the game
const unsigned char gc_ucFPS = 60;                // FPS of this game
const unsigned int gc_uFrameTime = 1000 / gc_ucFPS;    // time for each frame

//main loop declaration
void mainLoop(Entity* playerPtr, Entity* chadPtr, Entity* copPtr, Entity* customerPtr, Entity* hoarderPtr, Player& player, Chad& chad, Cop& cop, Customer& customer, Hoarder& hoarder);

// TODO:
// Bug in waitUntil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

// main function - starting function
// You should not be modifying this unless you know what you are doing
int main( void )
{
    srand((unsigned int)time(NULL));
    SKeyEvent g_skKeyEvent[K_COUNT];
    Entity* playerPtr;
    Entity* chadPtr;
    Entity* copPtr;
    Entity* customerPtr;
    Entity* hoarderPtr;
    Player player;
    Chad chad;
    Cop cop;
    Customer customer;
    Hoarder hoarder;
    playerPtr = &player;
    chadPtr = &chad;
    copPtr = &cop;
    customerPtr = &customer;
    hoarderPtr = &hoarder;
    init(playerPtr, player, chad, customer); // initialize your variables
    mainLoop(playerPtr, chadPtr, copPtr, customerPtr, hoarderPtr, player, chad, cop, customer, hoarder);  // main loop
    shutdown();  // do clean up, if any. free memory.
    delete playerPtr;
    delete chadPtr;
    delete copPtr;
    delete customerPtr;
    delete hoarderPtr;
    return 0;
}

//--------------------------------------------------------------
// Purpose  : This main loop calls functions to get input, 
//            update and render the game at a specific frame rate
//            You should not be modifying this unless you know what you are doing.
// Input    : void
// Output   : void
//--------------------------------------------------------------
void mainLoop(Entity *playerPtr, Entity *chadPtr, Entity *copPtr, Entity *customerPtr, Entity *hoarderPtr, Player &player, Chad &chad, Cop &cop, Customer &customer, Hoarder &hoarder)
{
    g_Timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!g_bQuitGame)      // run this loop until user wants to quit 
    {        
        getInput();                         // get keyboard input
        update(g_Timer.getElapsedTime(), playerPtr, chadPtr, customerPtr, player, chad, customer); // update the game
        render(playerPtr, chadPtr, copPtr, customerPtr, hoarderPtr, player, chad, cop, customer, hoarder); // render the graphics output to screen
        g_Timer.waitUntil(gc_uFrameTime);   // Frame rate limiter. Limits each frame to a specified time in ms.      
    }    
}