#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include "Entity.h"
#include "Item.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
//struct SMouseEvent
//{
//    COORD mousePosition;
//    DWORD buttonState;
//    DWORD eventFlags;
//};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_W,
    K_S,
    K_A,
    K_D,
    K_SHIFT,
    K_SPACE,
    K_ESCAPE,
    K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_MAINMENU,
    S_GAME,
    S_GAMEOVER,
    S_TITLE,
    S_TUTORIAL,
    S_COUNT
};

enum EMENUSTATE
{
    S_MENU1,
    S_MENU2,
    S_MENU3,
    S_OPTION1,
    S_OPTION2,
    S_RESET
};
// struct for the game characters
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void updateMenu();
void resetScore();
void titleWait();
void updateGame(double dt);          // gameplay logic
void updateTutorial(double dt);
void gameOverWait();        // waits for time to pass in gameover screen
// void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderMainMenu();  // renders the main menu
void renderTitle();
void renderGame();          // renders the game stuff
void renderGameOver();      // renders game over screen
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderEffect();
void renderNPC(Entity*);
void renderItem(Item* itemPtr);
//void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderHUD();
void renderBar();
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events
void chadPush();
void customerBlock();
void copBlock();
void checkLocation(Map &map, Entity*);
void checkItem(Map &map, Item*);

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
//void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
//void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay

#endif // _GAME_H