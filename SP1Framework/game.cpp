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
int spamIncrease;
bool spawnedTP = false;
bool isContesting = false;
bool isGameOver = false;

double  g_dElapsedTime;
double  g_dDeltaTime;
double  g_dPrevPlayerTime;
double  g_dPrevChadTime;
double  g_dPrevCustomerTime;
double  g_dCooldown;
SKeyEvent g_skKeyEvent[K_COUNT];
//SMouseEvent g_mouseEvent;

// Game specific variables here HELP
Entity*      playerPtr;
Player       player;
Item*        toiletPaper;

SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_TITLE; // initial state s
EMENUSTATE g_eMenuState = S_MENU1;
Map map;

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
    // Set precision for floating point output
    g_dElapsedTime = 60.0;

    chadCount = 0;
    copCount = 0;
    customerCount = 0;
    hoarderCount = 0;
    karenCount = 0;
    spamCount = 0;
    spamIncrease = 35;

    // sets the initial state for the game
    g_eGameState = S_TITLE;

    playerPtr = &player;
    playerPtr->setPos('x', 5);
    playerPtr->setPos('y', g_Console.getConsoleSize().Y / 2);
    entityList.push_back(playerPtr);
    toiletPaper = new Item();


    /*g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;*/
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    //g_Console.setMouseHandler(mouseHandler);
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

    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
//void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
//{    
//    switch (g_eGameState)
//    {
//    case S_MAINMENU: gameplayMouseHandler(mouseEvent);
//        break;
//    case S_GAME: // handle gameplay mouse event
//        break;
//    }
//}

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
    double time = g_dElapsedTime - g_dPrevPlayerTime;

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
        /*if (time > 0.2f) */
        {  //player.setKey(g_skKeyEvent);
            g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
            g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
            g_dPrevPlayerTime = g_dElapsedTime;
        }
    }    
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
//void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
//{
//    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
//    {
//        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
//    }
//    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
//    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
//}

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

    switch (g_eGameState)
    {
        case S_TITLE: titleWait();
            break;
        case S_MAINMENU: updateMenu(); //temp thing until we can get menu buttons to work
            break;
        case S_TUTORIAL: updateTutorial(dt);
            break;
        case S_GAME: updateGame(dt); // gameplay logic when we are in the game
            break;
        case S_GAMEOVER: gameOverWait(); // game logic for the gameover screen?
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
            g_eMenuState = S_MENU2;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
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
            g_eMenuState = S_MENU3;
            break;
        }
        else if (g_skKeyEvent[K_W].keyDown)
        {
            g_eMenuState = S_MENU1;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            g_eGameState = S_TUTORIAL;
            break;
        }
        else
        break;
    case S_MENU3:
        if (g_skKeyEvent[K_W].keyDown)
        {
            g_eMenuState = S_MENU2;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            g_eMenuState = S_OPTION1;
            break;
        }
        else
        break;
    case S_OPTION1:
        if (g_skKeyEvent[K_S].keyDown)
        {
            g_eMenuState = S_OPTION2;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
            g_dElapsedTime = 10; // temp timer
            g_eMenuState = S_RESET;
            break;
        }
        else
        break;
    case S_OPTION2:
        if (g_skKeyEvent[K_W].keyDown)
        {
            g_eMenuState = S_OPTION1;
            break;
        }
        else if (g_skKeyEvent[K_SPACE].keyDown)
        {
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
            item->removeItem(map);
        }
    }
    spawnedTP = false;
    delete item;
    toiletPaper = nullptr;
}

void resetScore()
{
    if (g_dElapsedTime > 2.0) // wait for 2 seconds to switch to menu mode, else do nothing
        g_eGameState = S_MAINMENU;
}

void titleWait()
{
    if (g_dElapsedTime < 57.0) // wait for 3 seconds to switch to menu mode, else do nothing
        g_eGameState = S_MAINMENU;
}

void updateTutorial(double dt)
{

}

void updateGame(double dt)       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    if(!isContesting)
        player.movement(map, g_skKeyEvent); // moves the character, collision detection, physics, etc

    if (spamIncrease >= 44)
    {
        isContesting = false;
        ++current_score;
        if (current_score > high_score)
            high_score = current_score;
        for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
        {
            Entity* entity = (Entity*)*it;
            if(entity->getState() == true)
                entity->setState(false);
        }
        spamIncrease = 35;
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && spamIncrease < 44)
    {
        ++spamCount;
    }
    if (spamCount > 4)
    {
        ++spamIncrease;
        spamCount = 0;
    }
    if (chadCount < 3)
    {
        Entity* chadPtr = new Chad;
        checkLocation(map, chadPtr);
        Chad* chad = dynamic_cast<Chad*>(chadPtr);
        chad->setPlayer(playerPtr);
        entityList.push_back(chadPtr);
        ++chadCount;
    }
    if (copCount < 2)
    {
        Entity* copPtr = new Cop;
        checkLocation(map, copPtr);
        Cop* cop = dynamic_cast<Cop*>(copPtr);
        cop->setPlayer(playerPtr);
        entityList.push_back(copPtr);
        ++copCount;
    }
    if (customerCount < 5)
    {
        Entity* customerPtr = new Customer;
        checkLocation(map, customerPtr);
        Customer* customer = dynamic_cast<Customer*>(customerPtr);
        customer->setPlayer(playerPtr);
        entityList.push_back(customerPtr);
        ++customerCount;
    }
    if (karenCount < 2)
    {
        Entity* karenPtr = new Karen;
        checkLocation(map, karenPtr);
        Karen* karen = dynamic_cast<Karen*>(karenPtr);
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
            checkItem(map, toiletPaper);
            map.setEntity(toiletPaper->getPos('x'), toiletPaper->getPos('y') - 1, (char)8);
            spawnedTP = true;
        }
    }
    if (hoarderCount < 1)
    {
        Entity* hoarderPtr = new Hoarder;
        checkLocation(map, hoarderPtr);
        //Hoarder* hoarder = dynamic_cast<Hoarder*>(hoarderPtr);
        //hoarder->solveAStar();
        entityList.push_back(hoarderPtr);
        ++hoarderCount;
    }

    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_HOARDER && !isContesting)
        {
            Hoarder* hoarder = dynamic_cast<Hoarder*>(entity);
            if (toiletPaper != nullptr)
            {
                hoarder->createPath(map);
                hoarder->setStart(toiletPaper->getPos('x'), toiletPaper->getPos('y'));
                hoarder->solveAStar();
            }
            hoarder->movement(map, dt);
        }
        else if (entity->getType() == Entity::TYPE_KAREN && !isContesting)
        {
            Karen* karen = dynamic_cast<Karen*>(entity);
            karen->createPath(map);
            if (karen->getIsEnd() == true)
            {
                karen->setStart(map);
                karen->setIsEnd(false);
            }
            karen->solveAStar();
            karen->move(map, dt);
        }
        else if (entity->getType() != Entity::TYPE_COP && !isContesting)
            entity->move(map, dt);
        if (entity->getType() == Entity::TYPE_CHAD)
        {
            Chad* chad = dynamic_cast<Chad*>(entity);
            if (chad->checkCollision())
            {
                chadPush();
                player.setActive(true);
                playerPtr->setCharColor(chad->getCharColor());
                renderCharacter();
            }
        }
        if (entity->getType() == Entity::TYPE_CUSTOMER)
        {
            Customer* customer = dynamic_cast<Customer*>(entity);
            if (customer->checkCollision())
            {
                customerBlock();
                player.setActive(true);
                playerPtr->setCharColor(customer->getCharColor());
                renderCharacter();
            }
        }

        if (entity->getType() == Entity::TYPE_COP)
        {
            Cop* cop = dynamic_cast<Cop*>(entity);
            if (cop->checkCollision())
            {
                copBlock();
                player.setActive(true);
                playerPtr->setCharColor(cop->getCharColor());
                renderCharacter();
            }
        }

        if (spawnedTP) {
            if (entity->getPos('x') == toiletPaper->getPos('x') && entity->getPos('y') == toiletPaper->getPos('y')) {
                if (entity->getType() != 0)
                    entity->setState(true);
                pickedUpItem(map, toiletPaper, entity, player);
            }
        }
        if (contest(entity, playerPtr))
        {
            isContesting = true;
        }
    }

    double coolDown = g_dElapsedTime - g_dCooldown;
    if (player.getSpeed() && coolDown > 5.0f)
    {
        player.setSpeed(false);
        g_dCooldown = g_dElapsedTime;
    }
    //chadPush(); // checks if chad pushes player
    //customer->move();
    //customerBlock();
    //moveCharacter();    
                        // sound can be played here too.
}

void gameOverWait()
{
    if (g_dElapsedTime < -5.0) // wait for 5 seconds to switch to main menu, else do nothing
        g_eGameState = S_MAINMENU;
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;  
    if (g_dElapsedTime < 0.0)
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
    case S_GAME: renderGame();
        renderHUD(); // renders debug information, frame rate, elapsed time, etc
        break;
    case S_GAMEOVER: renderGameOver();
        break;
    } 
    //renderInputEvents();    // renders status of input events
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
}

void renderTitle()
{
    COORD t;
    t.X = 7;
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

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    /*for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        renderNPC(entity);
        if (isContesting)
            renderBar();
    }*/
    if (spawnedTP)
        renderItem(toiletPaper);
}

void freeMemory(Entity* hoarder, Entity* player, Entity* customer, Entity* chad, Entity* cop, Entity* karen, Item* toiletPaper)
{
    delete hoarder;
    delete player;
    delete customer;
    delete chad;
    delete cop;
    delete karen;
    delete toiletPaper;
}

void renderGameOver()
{
    COORD t;
    t.X = 25;
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

    //for (int i = 1; i < entityList.size(); ++i)
    //{
    //    //Entity* entity = entityList.back();
    //    delete entityList[i];
    //    //entityList.pop_back();
    //}
    //entityList.clear();
    
}

void renderCamera(COORD camera, int lowX, int lowY, int highX, int highY)
{
    const WORD colors[] = {
    0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
    0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6,
    00
    };
    for (int r = lowY; r < highY; r++)
    {
        camera.Y = r + 1;
        for (int c = lowX; c < highX; c++)
        {
            camera.X = c;
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
        }
    }
}

void renderMap()
{
    // Set up sample colours, and output shadings
    COORD camera;
    /*for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }*/
    //Map map;
    camera.X = playerPtr->getPos('x') - 12;
    camera.Y = playerPtr->getPos('y') - 5;
    if (camera.X < 12)
        camera.X = 12;
    else if (camera.X > g_Console.getConsoleSize().X - 12)
        camera.X = g_Console.getConsoleSize().X - 12;
    if (camera.Y < 5)
        camera.Y = 5;
    else if (camera.Y > g_Console.getConsoleSize().Y - 5)
        camera.Y = g_Console.getConsoleSize().Y - 5;
    renderCamera(camera, playerPtr->getPos('x') - 12, playerPtr->getPos('y') - 5, playerPtr->getPos('x') + 13, playerPtr->getPos('y') + 4);
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_HOARDER || entity->getType() == Entity::TYPE_KAREN)
        {
            camera.X = entity->getPos('x') - 5;
            camera.Y = entity->getPos('y') - 2;
            if (camera.X < 5)
                camera.X = 5;
            else if (camera.X > g_Console.getConsoleSize().X - 5)
                camera.X = g_Console.getConsoleSize().X - 5;
            if (camera.Y < 3)
                camera.Y = 3;
            else if (camera.Y > g_Console.getConsoleSize().Y - 3)
                camera.Y = g_Console.getConsoleSize().Y - 3;
            renderCamera(camera, entity->getPos('x') - 5, entity->getPos('y') - 3, entity->getPos('x') + 6, entity->getPos('y') + 2);
        }
    }
    /*else if (map.getEntity(R, C) == 'K')
        g_Console.writeToBuffer(c, 'K', 0xDF);
    else if (map.getEntity(R, C) == 'C')
        g_Console.writeToBuffer(c, 'C', 0x0F);
    else if (map.getEntity(R, C) == 'P')
        g_Console.writeToBuffer(c, 'P', 0x1F);
    else if (map.getEntity(R, C) == (char)4)
        g_Console.writeToBuffer(c, (char)4, 0x0C);
    else if (map.getEntity(R, C) == 'H')
        g_Console.writeToBuffer(c, 'H', 0x06);
    else if (map.getEntity(R, C) == (char)8)
        g_Console.writeToBuffer(c, (char)8, 0x6F);
    else if (map.getEntity(R, C) == (char)22)
        g_Console.writeToBuffer(c, (char)22, 0x6F);
    else if (map.getEntity(R, C) == (char)43)
        g_Console.writeToBuffer(c, (char)43, 0x6F);
    else if (map.getEntity(R, C) == (char)127)
        g_Console.writeToBuffer(c, (char)127, 0x6F);
    else if (map.getEntity(R, C) == (char)13)
        g_Console.writeToBuffer(c, (char)13, 0x6F);
    else if (map.getEntity(R, C) == (char)7)
        g_Console.writeToBuffer(c, (char)7, 0x6F);*/
        //else
            //g_Console.writeToBuffer(c, 'n', colors[12]);
}

void renderCharacter()
{
    COORD temp;
    temp.X = playerPtr->getPos('x');
    temp.Y = playerPtr->getPos('y');
    // Draw the location of the character
    g_Console.writeToBuffer(temp, (char)21, playerPtr->getCharColor());
}

//void renderEffect()
//{
//    COORD temp;
//    for (int C = 0; C < 24; C++)
//    {
//        temp.Y = C + 1;
//        for (int R = 0; R < 80; R++)
//        {
//            temp.X = R;
//            if (map.getEntity(R, C) == (char)23)
//                g_Console.writeToBuffer(temp, (char)23, );
//        }
//    }
//}

void renderNPC(Entity* entity)
{
    COORD temp;
    temp.X = entity->getPos('x');
    temp.Y = entity->getPos('y');
    switch (entity->getType())
    {
    case Entity::TYPE_CHAD:
        //map.setEntity(temp.X, temp.Y, 'B');
        g_Console.writeToBuffer(temp, (char)4, entity->getCharColor());
        break;
    case Entity::TYPE_COP:
        //map.setEntity(temp.X, temp.Y, 'P');
        g_Console.writeToBuffer(temp, 'P', entity->getCharColor());
        break;
    case Entity::TYPE_CUSTOMER:
        //map.setEntity(temp.X, temp.Y, 'C');
        g_Console.writeToBuffer(temp, 'C', entity->getCharColor());
        break;
    case Entity::TYPE_HOARDER:
        //map.setEntity(temp.X, temp.Y, 'H');
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
    COORD pos2 = { spamIncrease,24 };
    g_Console.writeToBuffer(pos2, (char)178, 0x2B);
}

// this is an example of how you would use the input eventss
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

    // mouse events    
    /*ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:        
        break;
    }*/
    
}

void chadPush()
{
    int playerX = playerPtr->getPos('x');
    int playerY = playerPtr->getPos('y');
    if (player.getDirection() == 0)                                     // UP
    {
        if (playerY + 3 < 23) {
            if (map.getEntity(playerX, playerY + 1) != 'w'
                && map.getEntity(playerX, playerY + 2) != 'w'
                && map.getEntity(playerX, playerY + 3) != 'w')
                playerPtr->setPos('y', playerY + 3);
        }
    }
    else if (player.getDirection() == 1)                                // LEFT
    {
        if (playerX + 4 < 79) {
            if (map.getEntity(playerX + 1, playerY) != 'w'
                && map.getEntity(playerX + 2, playerY) != 'w'
                && map.getEntity(playerX + 3, playerY) != 'w'
                && map.getEntity(playerX + 4, playerY) != 'w')
                playerPtr->setPos('x', playerX + 4);
        }
    }
    else if (player.getDirection() == 2)                                // DOWN
    {
        if (playerY - 3 > 1) {
            if (map.getEntity(playerX, playerY - 1) != 'w'
                && map.getEntity(playerX, playerY - 2) != 'w'
                && map.getEntity(playerX, playerY - 3) != 'w')
                playerPtr->setPos('y', playerY - 3);
        }
    }
    else if (player.getDirection() == 3)                                // RIGHT
    {
        if (playerX - 4 > 1) {
            if (map.getEntity(playerX - 1, playerY) != 'w'
                && map.getEntity(playerX - 2, playerY) != 'w'
                && map.getEntity(playerX - 3, playerY) != 'w'
                && map.getEntity(playerX - 4, playerY) != 'w')
                playerPtr->setPos('x', playerX - 4);
        }
    }
}

void customerBlock()
{
    if (player.getDirection() == 0 && player.getPos('y') + 1 < 24)
        playerPtr->setPos('y', playerPtr->getPos('y') + 1);
    else if (player.getDirection() == 1 && player.getPos('x') + 1 < 79)
        playerPtr->setPos('x', playerPtr->getPos('x') + 1);
    else if (player.getDirection() == 2 && player.getPos('y') - 1 > 1)
        playerPtr->setPos('y', playerPtr->getPos('y') - 1);
    else if (player.getDirection() == 3 && player.getPos('x') - 1 > 1)
        playerPtr->setPos('x', playerPtr->getPos('x') - 1);
}

void copBlock()
{
    if (player.getDirection() == 0 && player.getPos('y') + 1 < 24)
        playerPtr->setPos('y', playerPtr->getPos('y') + 1);
    else if (player.getDirection() == 1 && player.getPos('x') + 1 < 79)
        playerPtr->setPos('x', playerPtr->getPos('x') + 1);
    else if (player.getDirection() == 2 && player.getPos('y') - 1 > 1)
        playerPtr->setPos('y', playerPtr->getPos('y') - 1);
    else if (player.getDirection() == 3 && player.getPos('x') - 1 > 1)
        playerPtr->setPos('x', playerPtr->getPos('x') - 1);
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