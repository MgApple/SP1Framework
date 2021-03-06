// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Chad.h"
#include "Cop.h"
#include "Customer.h"
#include "Hoarder.h"
#include "Item.h"
#include "Karen.h"
#pragma comment(lib, "winmm.lib")

int high_score;
int current_score;
std::vector<Entity*> entityList;
int chadCount;
int copCount;
int customerCount;
int hoarderCount;
int karenCount;
int spamCount;
int spamPos;
bool spawnedTP = false;
bool isContesting = false;
bool isGameOver = false;
bool playerCheck = false;
COORD temp;

double  g_dElapsedTime;
double  g_dDeltaTime;
double  g_dPlayerTime;
double  g_dPrevPlayerTime;
double  g_dFrozen;
SKeyEvent g_skKeyEvent[K_COUNT];

// Game specific variables here
Entity*      playerPtr;
Player       player;
Item*        toiletPaper;

SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_TITLE; // initial state s
EMENUSTATE g_eMenuState = S_MENU1;
Map map;
Map tutorial;

// Console object
Console g_Console(80, 25, "Market Blackout");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    std::string save;
    std::ifstream savefile;
    savefile.open("save.txt");
    if (savefile) {
        while (getline(savefile, save))
        {
            if (save.substr(0, 11) == "high_score:")
                high_score = stoi(save.substr(save.find(':') + 1));
        }
        savefile.close();
    }
    else {
        std::ofstream newsave("save.txt");
        newsave << "high_score:0";
        high_score = 0;
    }
    map.loadMap();
    tutorial.loadtutorial();
    // Set precision for floating point output
    g_dElapsedTime = 60.0;
    g_dPlayerTime = 0.0;
    g_dFrozen = 0.0;

    chadCount = 0;
    copCount = 0;
    customerCount = 0;
    hoarderCount = 0;
    karenCount = 0;
    spamCount = 0;
    spamPos = 35;

    // sets the initial state for the game
    g_eGameState = S_TITLE;

    playerPtr = &player;
    playerPtr->setPos('x', 5);
    playerPtr->setPos('y', g_Console.getConsoleSize().Y / 2);
    entityList.push_back(playerPtr);
    toiletPaper = new Item();

    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Voids
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{    
    switch (g_eGameState)
    {
    case S_MAINMENU: gameplayKBHandler(keyboardEvent);
        break;
    case S_TUTORIAL:gameplayKBHandler(keyboardEvent);
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    //double time = g_dPlayerTime - g_dPrevPlayerTime;

    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case 0x57: key = K_W; break;
    case 0x53: key = K_S; break;
    case 0x41: key = K_A; break;
    case 0x44: key = K_D; break;
    case SHIFT_PRESSED: key = K_SHIFT; break;
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        //if (time > 0.5) 
        {  
            g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
            g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
            g_dPrevPlayerTime = g_dPlayerTime;
        }
    }    
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime -= dt;
    g_dDeltaTime = dt;
    g_dPlayerTime += dt;
    if (g_dFrozen >= 0)
        g_dFrozen -= dt;
    switch (g_eGameState)
    {
        case S_TITLE: titleWait();
            break;
        case S_MAINMENU: updateMenu();
            break;
        case S_TUTORIAL: updateGame(dt, tutorial);
            break;
        case S_GAME: updateGame(dt, map); // gameplay logic when we are in the game
            break;
        case S_GAMEOVER: gameOverWait(); // game logic for the gameover screen
            break;
    }
}


void updateMenu()
{
    switch (g_eMenuState)
    {
    case S_MENU1:
        if (g_skKeyEvent[K_S].keyDown)
        {
            PlaySound(TEXT("menu1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_MENU2;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            PlaySound(TEXT("menu2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_dElapsedTime = 60.0; //reset timer
            current_score = 0;
            g_eGameState = S_GAME;
            break;
        }
        else
        break;
    case S_MENU2:
        if (g_skKeyEvent[K_S].keyDown)
        {
            PlaySound(TEXT("menu1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_MENU3;
            break;
        }
        else if (g_skKeyEvent[K_W].keyDown)
        {
            PlaySound(TEXT("menu1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_MENU1;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            PlaySound(TEXT("menu2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eGameState = S_TUTORIAL;
            break;
        }
        else
        break;
    case S_MENU3:
        if (g_skKeyEvent[K_W].keyDown)
        {
            PlaySound(TEXT("menu1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_MENU2;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            PlaySound(TEXT("menu2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_OPTION1;
            break;
        }
        else
        break;
    case S_OPTION1:
        if (g_skKeyEvent[K_S].keyDown)
        {
            PlaySound(TEXT("menu1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_OPTION2;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            PlaySound(TEXT("menu2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_dElapsedTime = 10; // temp timer
            g_eMenuState = S_RESET;
            break;
        }
        else
        break;
    case S_OPTION2:
        if (g_skKeyEvent[K_W].keyDown)
        {
            PlaySound(TEXT("menu1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_OPTION1;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            PlaySound(TEXT("menu2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            g_eMenuState = S_MENU3;
            break;
        }
        else
        break;
    case S_RESET:
        high_score = 0;
        if (g_dElapsedTime < 7.0)
            g_eMenuState = S_OPTION1;
        break;
    }
    
}

void pickedUpItem(Map& map, Item* item, Entity* entity, Player& player)
{
    if (entity->getType() == 0) {
        PlaySound(TEXT("sfx_point.wav"), NULL, SND_FILENAME | SND_ASYNC);
        ++current_score;
        if (current_score > high_score)
            high_score = current_score;
        g_dElapsedTime += 5.0;
        item->removeItem(map);
    }
    else {
        if (!(entity->getState())) { // if is not holding toilet paper
            entity->setState(true);
            PlaySound(TEXT("stolen.wav"), NULL, SND_FILENAME | SND_ASYNC);
            if (entity->getType()!=Entity::TYPE_HOARDER && entity->getType() != Entity::TYPE_KAREN)
                item->removeItem(map);
        }
    }
    spawnedTP = false;
    delete item;
    toiletPaper = nullptr;
}

void titleWait()
{
    if (g_dElapsedTime > 59.9)
        PlaySound(TEXT("title.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (g_dElapsedTime < 57.0) // wait for 3 seconds to switch to menu mode, else do nothing
        g_eGameState = S_MAINMENU;
}

void updateGame(double dt, Map &map)       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    if (!isContesting && g_dFrozen <= 5.0)
        player.movement(map, g_skKeyEvent); // moves the character

    // for contesting and stealing from other customers
    if (spamPos >= 44) // if the spam bar is full
    {
        isContesting = false;
        PlaySound(TEXT("sfx_point.wav"), NULL, SND_FILENAME | SND_ASYNC);
        ++current_score;
        if (current_score > high_score)
            high_score = current_score;
        for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
        {
            Entity* entity = (Entity*)*it;
            if(entity->getState() == true)
                entity->setState(false); // reset all entity state
        }
        spamPos = 35; // reset position back to original position
        g_dElapsedTime += 5.0;
    }
    // if they are contesting and the space bar is being pressed, increase the count
    if (g_skKeyEvent[K_SPACE].keyReleased && spamPos < 44 && isContesting) 
    {
        ++spamCount;
    }
    // each unit has to be spammed 5 times for it to increase position
    if (spamCount > 4)
    {
        PlaySound(TEXT("minigame.wav"), NULL, SND_FILENAME | SND_ASYNC);
        ++spamPos;
        spamCount = 0;
    }
    // going through entityList
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_HOARDER)
        {
            Hoarder* hoarder = dynamic_cast<Hoarder*>(entity);
            if (!isContesting)
            {
                hoarder->updatePath(map);
                if (toiletPaper != nullptr)
                {
                    hoarder->setStart(toiletPaper->getPos('x'), toiletPaper->getPos('y'));
                    hoarder->solveAStar();
                }
                hoarder->movement(map, dt);
                if (hoarder->checkCollision(playerPtr))
                {
                    if (g_eGameState == S_TUTORIAL) {
                        COORD temp;
                        std::ostringstream ss;
                        ss << "A fellow Hoarder trying to beat you at your own game, they will try to grab the toilet paper before you do." << "PRESS SPACE TO CONTINUE";
                        temp.X = 15;
                        temp.Y = 12;
                        g_Console.writeToBuffer(temp, ss.str());
                    }
                    enemyBlock(map,entity);
                    renderCharacter();
                }
                temp.X = hoarder->getPos('x');
                temp.Y = hoarder->getPos('y');
            }
            else
            {
                hoarder->setPos('x', hoarder->getPos('x'));
                hoarder->setPos('y', hoarder->getPos('y'));
            }
        }
        else if (entity->getType() == Entity::TYPE_KAREN && !isContesting)
        {
            Karen* karen = dynamic_cast<Karen*>(entity);
            if (playerCheck == true && g_dFrozen <= 0.0 && karen->aggro(playerPtr, map))
            {
                g_dFrozen = 8.0;
                PlaySound(TEXT("karen_sfx.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
            else if (g_dFrozen <= 5.0)
            {
                karen->updatePath(map);
                if (karen->getIsEnd() == true)
                {
                    karen->setStart(map);
                    karen->setIsEnd(false);
                }
                karen->solveAStar();
                karen->move(map, dt);
                if (karen->checkCollision(playerPtr))
                {
                    if (g_eGameState == S_TUTORIAL) {
                        COORD temp;
                        std::ostringstream ss;
                        ss << "A feisty Mother who just wants to argue, freezes you in place for 3 seconds to argue." << "PRESS SPACE TO CONTINUE";
                        temp.X = 15;
                        temp.Y = 12;
                        g_Console.writeToBuffer(temp, ss.str());
                    }
                    enemyBlock(map,entity);
                    renderCharacter();
                }
            }
        }
        else if (entity->getType() != Entity::TYPE_COP && !isContesting)
            entity->move(map, dt);
        if (entity->getType() == Entity::TYPE_CHAD)
        {
            Chad* chad = dynamic_cast<Chad*>(entity);
            if (chad->checkCollision(playerPtr))
            {
                if (g_eGameState == S_TUTORIAL) {
                    COORD temp;
                    std::ostringstream ss;
                    ss << "A jerk who likes to push people when you bump into him, pushes you away when you collide with them." << "PRESS SPACE TO CONTINUE";
                    temp.X = 15;
                    temp.Y = 12;
                    g_Console.writeToBuffer(temp, ss.str());
                }   
                chadPush(map);
                renderCharacter();
            }
        }
        if (entity->getType() == Entity::TYPE_CUSTOMER)
        {
            Customer* customer = dynamic_cast<Customer*>(entity);
            if (customer->checkCollision(playerPtr))
            {
                if (g_eGameState == S_TUTORIAL) {
                    COORD temp;
                    std::ostringstream ss;
                    ss << "A normal person who may or may not block your path." << "PRESS SPACE TO CONTINUE";
                    temp.X = 15;
                    temp.Y = 12;
                    g_Console.writeToBuffer(temp, ss.str());
                }
                enemyBlock(map,entity);
                renderCharacter();
            }
        }

        if (entity->getType() == Entity::TYPE_COP)
        {
            Cop* cop = dynamic_cast<Cop*>(entity);
            if (cop->checkCollision(playerPtr))
            {
                if (g_eGameState == S_TUTORIAL) {
                    COORD temp;
                    std::ostringstream ss;
                    ss << "A mall cop trying to do his job, stands and blocks certain paths at random." << "PRESS SPACE TO CONTINUE";
                    temp.X = 15;
                    temp.Y = 12;
                    g_Console.writeToBuffer(temp, ss.str());
                }
                enemyBlock(map,entity);
                renderCharacter();
            }
        }

        if (spawnedTP) {
            if (entity->getPos('x') == toiletPaper->getPos('x') && entity->getPos('y') == toiletPaper->getPos('y')) {
                if (entity->getType() != 0 && entity->getType()!=2 && entity->getType()!=6)
                {
                    entity->setState(true);
                }
                else if (entity->getType() == 0)
                {
                    if (g_eGameState == S_TUTORIAL) {
                        COORD temp;
                        std::ostringstream ss;
                        ss << "The objective, collect as many as you can before time runs out." << "PRESS SPACE TO CONTINUE";
                        temp.X = 15;
                        temp.Y = 12;
                        g_Console.writeToBuffer(temp, ss.str());
                    }
                }
                pickedUpItem(map, toiletPaper, entity, player);
            }
        }
        if (contest(entity, playerPtr))
        {
            isContesting = true;
        }
    }

}

void gameOverWait()
{
    if (g_dElapsedTime < 0 && g_dElapsedTime > -0.1)
        PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (g_dElapsedTime < -5.0) // wait for 5 seconds to switch to main menu, else do nothing
        g_eGameState = S_MAINMENU;
}

void processUserInput()
{
    // goes to main menu if player is in tutorial and hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyDown && g_eGameState == S_TUTORIAL)
        g_eGameState = S_MAINMENU;
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;
    if (g_dElapsedTime < 0.0)
        if (g_dElapsedTime < 0.0 && g_eGameState != S_TUTORIAL)
            g_eGameState = S_GAMEOVER;
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_TITLE: renderTitle();
        break;
    case S_MAINMENU: renderMainMenu();
        break;
    case S_TUTORIAL:
        renderTutorial();
        break;
    case S_GAME: 
        renderGame();
        renderHUD(); // renders debug information, frame rate, elapsed time, etc
        break;
    case S_GAMEOVER: renderGameOver();
        break;
    } 
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(00);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderMainMenu()  // renders the main menu
{
    COORD c;
    c.X = 1;
    c.Y = 2;
    std::ifstream menu;
    std::string line;
    switch (g_eMenuState)
    {
    case S_MENU1:
        menu.open("menu.txt");
        if (menu) {
            while (getline(menu, line)) {
                g_Console.writeToBuffer(c, line);
                c.Y += 1;
            }
        }
        break;
    case S_MENU2:
        menu.open("menu2.txt");
        if (menu) {
            while (getline(menu, line)) {
                g_Console.writeToBuffer(c, line);
                c.Y += 1;
            }
        }
        break;
    case S_MENU3:
        menu.open("menu3.txt");
        if (menu) {
            while (getline(menu, line)) {
                g_Console.writeToBuffer(c, line);
                c.Y += 1;
            }
        }
        break;
    case S_OPTION1:
        menu.open("option.txt");
        if (menu) {
            while (getline(menu, line)) {
                g_Console.writeToBuffer(c, line);
                c.Y += 1;
            }
        }
        break;
    case S_OPTION2:
        menu.open("option2.txt");
        if (menu) {
            while (getline(menu, line)) {
                g_Console.writeToBuffer(c, line);
                c.Y += 1;
            }
        }
        break;
    case S_RESET:
        c.X = 6;
        c.Y = 5;
        menu.open("reset.txt");
        if (menu) {
            while (getline(menu, line)) {
                g_Console.writeToBuffer(c, line);
                c.Y += 1;
            }
        }
        break;
    }
    freeMemory();
}

void renderTitle()
{
    COORD t;
    t.X = 12;
    t.Y = 5;
    std::ifstream title;
    std::string line;
    title.open("TITLE.txt");
    if (title) {
        while (getline(title, line)) {
            g_Console.writeToBuffer(t, line);
            t.Y += 1;
        }
    }
}

void renderTutorial()
{
    renderTutorialMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    if (spawnedTP)
        renderItem(toiletPaper);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    if (spawnedTP)
        renderItem(toiletPaper);
}

void freeMemory()
{
    // deleting and resetting
    for (int i = 1; i < entityList.size(); ++i)
    {
        delete entityList[i];
    }
    entityList.erase(entityList.begin() + 1, entityList.end());
    playerPtr->setPos('x', 5);
    playerPtr->setPos('y', g_Console.getConsoleSize().Y / 2);
    //checkItem(map, toiletPaper);
    //checkItem(map, toiletPaper);
    isContesting = false;
    chadCount = 0;
    copCount = 0;
    customerCount = 0;
    hoarderCount = 0;
    karenCount = 0;
    spamCount = 0;
    spamPos = 35;
    tutorial.reloadtutorial();
    tutorial.loadtutorial();
    map.reloadMap();
    map.loadMap();
}

void renderGameOver()
{
    COORD t;
    t.X = 26;
    t.Y = 2;
    std::ifstream gameover;
    std::string line;
    gameover.open("gameover.txt");
    if (gameover) {
        while (getline(gameover, line)) {
            g_Console.writeToBuffer(t, line);
            t.Y += 1;
        }
        std::ostringstream ss;
        t.Y += 1;
        ss << "Personal Best: " << high_score;
        t.X += 4;
        g_Console.writeToBuffer(t, ss.str());
        ss.str("");
        t.Y += 2;
        ss << "Score: " << current_score;
        g_Console.writeToBuffer(t, ss.str());
    }
    gameover.close();
    std::ofstream savefile("save.txt");
    savefile << "high_score:" << high_score;
    savefile.close();
}

void renderCamera(COORD camera, Map& map, int lowX, int lowY, int highX, int highY, bool karencheck)
{
    const WORD colors[] = {
    0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
    0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6,
    };
    if (karencheck==true)
        playerCheck = false;
    for (int r = lowY; r < highY; r++)
    {
        camera.Y = r + 1;
        if (r < 0 || r>24)
            continue;
        for (int c = lowX; c < highX; c++)
        {
            camera.X = c;
            if (c < 0 || c>80)
                continue;
            for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
            {
                Entity* entity = (Entity*)*it;
                if (entity->getPos('x') == camera.X && entity->getPos('y') == camera.Y)
                    renderNPC(entity);
                
                if (isContesting)
                    renderBar();
            }
            if (map.getEntity(c, r) == 'w')
                g_Console.writeToBuffer(camera, (char)219, colors[4]);
            else if (map.getEntity(c, r) == ' ')
                g_Console.writeToBuffer(camera, (char)32, colors[6]);
            if (karencheck==true && playerPtr->getPos('x') == camera.X && playerPtr->getPos('y') == camera.Y)
                playerCheck = true;
        }
    }
}

void renderMap()
{
    if (chadCount < 5)
    {
        Entity* chadPtr = new Chad;
        checkLocation(map, chadPtr);
        entityList.push_back(chadPtr);
        ++chadCount;
    }
    if (copCount < 7)
    {
        Entity* copPtr = new Cop;
        checkLocation(map, copPtr);
        entityList.push_back(copPtr);
        ++copCount;
    }
    if (customerCount < 10)
    {
        Entity* customerPtr = new Customer;
        checkLocation(map, customerPtr);
        entityList.push_back(customerPtr);
        ++customerCount;
    }
    if (karenCount < 1)
    {
        Entity* karenPtr = new Karen;
        checkLocation(map, karenPtr);
        Karen* karen = dynamic_cast<Karen*>(karenPtr);
        karen->createPath(map);
        entityList.push_back(karenPtr);
        ++karenCount;
    }
    if (!spawnedTP)
    {
        bool isBeingHeld = false;
        for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it) {
            Entity* entity = (Entity*)*it;
            if (entity->getState()) {
                isBeingHeld = true;
                COORD camera;
                camera.X = entity->getPos('x') - 2;
                camera.Y = entity->getPos('y') - 1;
                renderCamera(camera, map, camera.X, camera.Y-1, entity->getPos('x') + 3, entity->getPos('y') + 1 );
            }
        }
        if (!isBeingHeld) {
            toiletPaper = new Item();
            checkItem(map, toiletPaper);
            map.setEntity(toiletPaper->getPos('x'), toiletPaper->getPos('y') - 1, (char)8);
            spawnedTP = true;
        }
    }
    if (hoarderCount < 1)
    {
        Entity* hoarderPtr = new Hoarder;
        checkLocation(map, hoarderPtr);
        Hoarder* hoarder = dynamic_cast<Hoarder*>(hoarderPtr);
        hoarder->createPath(map);
        entityList.push_back(hoarderPtr);
        ++hoarderCount;
    }
    COORD camera;
    camera.X = playerPtr->getPos('x') - 12;
    camera.Y = playerPtr->getPos('y') - 5;
    renderCamera(camera, map, playerPtr->getPos('x') - 12, playerPtr->getPos('y') - 5, playerPtr->getPos('x') + 13, playerPtr->getPos('y') + 4);
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_KAREN)
        {
            camera.X = entity->getPos('x') - 7;
            camera.Y = entity->getPos('y') - 4;
            renderCamera(camera, map, entity->getPos('x') - 7, entity->getPos('y') - 4, entity->getPos('x') + 8, entity->getPos('y') + 3, true);
        }
        else if (entity->getType() == Entity::TYPE_HOARDER)
        {
            camera.X = entity->getPos('x') - 2;
            camera.Y = entity->getPos('y') - 1;
            renderCamera(camera,map, camera.X, camera.Y-1, entity->getPos('x') + 3, entity->getPos('y') + 1);
        }
    }
    if (spawnedTP) {
        camera.X = toiletPaper->getPos('x') - 2;
        camera.Y = toiletPaper->getPos('y') - 1;
        renderCamera(camera, map, camera.X, camera.Y-1, toiletPaper->getPos('x') + 3, toiletPaper->getPos('y') + 1);
    }
}

void renderTutorialMap()
{
    // this part creates the entities //
    if (chadCount < 1)
    {
        Entity* chadPtr = new Chad; // create new entity
        checkLocation(tutorial, chadPtr); // to check if it spawns in the wall
        entityList.push_back(chadPtr); // add the entity into entityList
        ++chadCount; // increase everytime an entity is made
    }
    if (copCount < 1)
    {
        Entity* copPtr = new Cop;
        checkLocation(tutorial, copPtr);
        entityList.push_back(copPtr);
        ++copCount;
    }
    if (customerCount < 1)
    {
        Entity* customerPtr = new Customer;
        checkLocation(tutorial, customerPtr);
        entityList.push_back(customerPtr);
        ++customerCount;
    }
    if (karenCount < 1)
    {
        Entity* karenPtr = new Karen;
        checkLocation(tutorial, karenPtr);
        Karen* karen = dynamic_cast<Karen*>(karenPtr);
        karen->createPath(tutorial);
        entityList.push_back(karenPtr);
        ++karenCount;
    }
    if (!spawnedTP)
    {
        bool isBeingHeld = false;
        for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it) {
            Entity* entity = (Entity*)*it;
            if (entity->getState()) {
                isBeingHeld = true;
            }
        }
        if (!isBeingHeld) {
            toiletPaper = new Item();
            checkItem(tutorial, toiletPaper);
            tutorial.setEntity(toiletPaper->getPos('x'), toiletPaper->getPos('y') - 1, (char)8);
            spawnedTP = true;
        }
    }
    if (hoarderCount < 1)
    {
        Entity* hoarderPtr = new Hoarder;
        checkLocation(tutorial, hoarderPtr);
        Hoarder* hoarder = dynamic_cast<Hoarder*>(hoarderPtr);
        hoarder->createPath(tutorial);
        entityList.push_back(hoarderPtr);
        ++hoarderCount;
    }
    // end of creating entities //
    COORD camera;
    camera.X = playerPtr->getPos('x') - 12;
    camera.Y = playerPtr->getPos('y') - 5;
    if (camera.X < 12)
        camera.X = 12;
    if (camera.Y < 5)
        camera.Y = 5;
    renderCamera(camera, tutorial,playerPtr->getPos('x') - 12, playerPtr->getPos('y') - 5, playerPtr->getPos('x') + 13, playerPtr->getPos('y') + 4);
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_KAREN)
        {
            camera.X = entity->getPos('x') - 5;
            camera.Y = entity->getPos('y') - 2;
            if (camera.X < 5)
                camera.X = 5;
            if (camera.Y < 3)
                camera.Y = 3;
            renderCamera(camera, tutorial,entity->getPos('x') - 5, entity->getPos('y') - 3, entity->getPos('x') + 6, entity->getPos('y') + 2,true);
        }
        else
        {
            camera.X = entity->getPos('x') - 2;
            camera.Y = entity->getPos('y') - 1;
            renderCamera(camera, tutorial, camera.X, camera.Y-1, entity->getPos('x') + 3, entity->getPos('y') + 1);
        }
    }
    if (spawnedTP) {
        camera.X = toiletPaper->getPos('x') - 2;
        camera.Y = toiletPaper->getPos('y') - 1;
        renderCamera(camera, tutorial, camera.X, camera.Y-1, toiletPaper->getPos('x') + 3, toiletPaper->getPos('y') + 1);
    }

    COORD temp;
    std::ostringstream ss;
    ss << "The objective, collect as many as you can before time runs out.";
    temp.X = 2;
    temp.Y = 2;
    g_Console.writeToBuffer(temp, ss.str());

    ss.str(" ");
    ss << "PRESS ESC to go back to main menu.";
    temp.X = 2;
    temp.Y = 3;
    g_Console.writeToBuffer(temp, ss.str());
}

void renderCharacter()
{
    COORD temp;
    temp.X = playerPtr->getPos('x');
    temp.Y = playerPtr->getPos('y');
    // Draw the location of the character
    g_Console.writeToBuffer(temp, (char)21, playerPtr->getCharColor());
}

void renderNPC(Entity* entity)
{
    COORD temp;
    temp.X = entity->getPos('x');
    temp.Y = entity->getPos('y');
    switch (entity->getType())
    {
    case Entity::TYPE_CHAD:
        g_Console.writeToBuffer(temp, (char)4, entity->getCharColor());
        break;
    case Entity::TYPE_COP:
        g_Console.writeToBuffer(temp, 'P', entity->getCharColor());
        break;
    case Entity::TYPE_CUSTOMER:
        g_Console.writeToBuffer(temp, 'C', entity->getCharColor());
        break;
    case Entity::TYPE_HOARDER:
        g_Console.writeToBuffer(temp, 'H', entity->getCharColor());
        break;
    case Entity::TYPE_KAREN:
        g_Console.writeToBuffer(temp, 'K', entity->getCharColor());
        break;
    }
}

void renderItem(Item* item)
{
    COORD temp;
    temp.X = item->getPos('x');
    temp.Y = item->getPos('y');
    map.setEntity(temp.X, temp.Y - 1, (char)8);
    g_Console.writeToBuffer(temp, (char)8, 0x6F);
}

void renderHUD()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = 71;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays high score
    ss.str("");
    ss << "High score:" << high_score;
    c.X = 15;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    ss.str("");
    ss << "Current score:" << current_score;
    c.X = 33;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());
}

void renderBar()
{
    COORD pos;
    for (int i = 0; i < 10; ++i)
    {
        pos.X = 35 + i;
        pos.Y = 24;
        g_Console.writeToBuffer(pos, (char)176, 0x2B);
    }
    COORD pos2 = { spamPos,24 };
    g_Console.writeToBuffer(pos2, (char)178, 0x2B);
}

void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
    for (short i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_W: key = "UP";
            break;
        case K_S: key = "DOWN";
            break;
        case K_A: key = "LEFT";
            break;
        case K_D: key = "RIGHT";
            break;
        case K_SHIFT: key = "SHIFT";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }    
}

void chadPush(Map& map)
{
    int playerX = playerPtr->getPos('x');
    int playerY = playerPtr->getPos('y');
    if (player.getDirection() == 0)                                     // UP
    {
        for (int i = 0; i < 3; i++)
        {
            playerY++;
            if (playerY == 24 || map.getEntity(playerX, playerY-1) != ' ')
            {
                playerY--;
                break;
            }
        }
    }
    else if (player.getDirection() == 1)                                // LEFT
    {
        for (int i = 0; i < 4; i++)
        {
            playerX++;
            if (playerX == 80 || map.getEntity(playerX, playerY-1) != ' ')
            {
                playerX--;
                break;
            }
        }
    }
    else if (player.getDirection() == 2)                                // DOWN
    {
        for (int i = 0; i < 3; i++)
        {
            playerY--;
            if (playerY == 0 || map.getEntity(playerX,playerY-1)!=' ')
            {
                playerY++;
                break;
            }
        }
    }
    else if (player.getDirection() == 3)                                // RIGHT
    {
        for (int i = 0; i < 4; i++)
        {
            playerX--;
            if (playerX == 0 || map.getEntity(playerX, playerY-1) != ' ')
            {
                playerX++;
                break;
            }
        }
    }
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {//checks if Chad still has same position as player and pushes him slightly to one side
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_CHAD) {
            Chad* chad = dynamic_cast<Chad*>(entity);
            if (chad->getPos('x')==playerX && chad->getPos('y')==playerY)
            {
                if (map.getEntity(playerX, playerY - 1 - 1) == ' ')
                    playerY--;
                else if (map.getEntity(playerX - 1, playerY - 1) == ' ')
                    playerX--;
                else if (map.getEntity(playerX + 1, playerY - 1) == ' ')
                    playerX++;
                else if (map.getEntity(playerX, playerY) == ' ')
                    playerY++;
            }
        }
    }
    playerPtr->setPos('x', playerX);
    playerPtr->setPos('y', playerY);
}

void enemyBlock(Map& map,Entity* entity)
{
    int check = 0;
    int playerX = playerPtr->getPos('x');
    int playerY = playerPtr->getPos('y');
    while (entity->getPos('x') == playerX && entity->getPos('y') == playerY)
    {
        if (((player.getDirection() == 0 && player.getPos('y') + 1 < 24) || check == 1) && map.getEntity(playerX, playerY)==' ')
        {
            playerY++;
        }
        else if (((player.getDirection() == 1 && player.getPos('x') + 1 < 79) || check == 2) && map.getEntity(playerX+1, playerY-1) == ' ')
        {
            playerX++;
        }
        else if (((player.getDirection() == 2 && player.getPos('y') - 1 > 1) || check == 3) && map.getEntity(playerX, playerY - 1-1) == ' ')
        {
            playerY--;
        }
        else if (((player.getDirection() == 3 && player.getPos('x') - 1 > 1) || check == 4) && map.getEntity(playerX - 1, playerY-1) == ' ')
        {
            playerX--;
        }
        else
            check++;
    }
    playerPtr->setPos('x', playerX);
    playerPtr->setPos('y', playerY);
}

void checkLocation(Map &map, Entity* entity)
{
    while (map.getEntity(entity->getPos('x'), entity->getPos('y') - 1) != ' ')
    {
        entity->reLoc();
    }
    if (entity->getType() == 2)
        map.setEntity(entity->getPos('x'), entity->getPos('y') - 1, 'K');
    else if (entity->getType() == 3)
        map.setEntity(entity->getPos('x'), entity->getPos('y') - 1, 'C');
    else if (entity->getType() == 4)
        map.setEntity(entity->getPos('x'), entity->getPos('y') - 1, 'P');
    else if (entity->getType() == 5)
        map.setEntity(entity->getPos('x'), entity->getPos('y') - 1, char(4));
    else if (entity->getType() == 6)
        map.setEntity(entity->getPos('x'), entity->getPos('y') - 1, 'H');
}

void checkItem(Map &map, Item* item)
{
    while (map.getEntity(item->getPos('x'), item->getPos('y') - 1) != ' ')
    {
        item->reLoc();
    }
}

bool contest(Entity* entity, Entity* player)
{
    int playerX = player->getPos('x');
    int playerY = player->getPos('y');
    int entityX = entity->getPos('x');
    int entityY = entity->getPos('y');

    if ((playerX == entityX && playerY - 1 == entityY) ||
        (playerX == entityX && playerY + 1 == entityY) ||
        (playerX - 1 == entityX && playerY == entityY) ||
        (playerX + 1 == entityX && playerY == entityY))
    {
        if(entity->getState() == true)
            return true;
    }
    else
        return false;
}