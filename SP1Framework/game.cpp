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

std::string save;
int high_score;
std::vector<Entity*> entityList;
int chadCount;
int copCount;
int customerCount;
int hoarderCount;
int itemCount;
int spamCount;
int spamIncrease;

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
Item*        itemPtr[4];

SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_TITLE; // initial state s
EMENUSTATE g_eMenuState = S_MENU1;
Map map;

// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
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
    g_dElapsedTime = 0.0;

    chadCount = 0;
    copCount = 0;
    customerCount = 0;
    hoarderCount = 0;
    spamCount = 0;
    spamIncrease = 30;

    // sets the initial state for the game
    g_eGameState = S_TITLE;

    playerPtr = &player;
    playerPtr->setPos('x', 1);
    playerPtr->setPos('y', g_Console.getConsoleSize().Y / 2);

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
    case 0x4A: key = K_J; break;
    case 0x4B: key = K_K; break;
    case 0x4C: key = K_L; break;
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
    g_dElapsedTime += dt;
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
    }
    
}

void pickedUpItem(Map& map, Item* item, Entity* entity, Player& player)
{
    switch (item->getItemType()) {
    case 1:
        if (!(entity->getState('t'))) { // if is not holding toilet paper
            entity->setState('t', true);
            item->removeItem(map);
        }
        break;
    case 2:
        if (entity->getType() == 0) { // if is a player
            for (int i = 0; i < 3; i++)
            {
                if (player.getInventory(i) == 0) { // if the player has an empty inventory slot
                    player.setInventory(i, 2);
                    item->removeItem(map);
                }
            }
        }
        else {
            item->removeItem(map);
        }
        break;
    case 3:
        if (entity->getType() == 0) {
            for (int i = 0; i < 3; i++)
            {
                if (player.getInventory(i) == 0) {
                    player.setInventory(i, 3);
                    item->removeItem(map);
                }
            }
        }
        else {
            item->removeItem(map);
        }
        break;
    case 4:
        if (entity->getType() == 0) {
            for (int i = 0; i < 3; i++)
            {
                if (player.getInventory(i) == 0) {
                    player.setInventory(i, 4);
                    player.setPState('p', true);
                    item->removeItem(map);
                }
            }
        }
        else {
            item->removeItem(map);
        }
        break;
    case 5:
        if (entity->getType() == 0) {
            for (int i = 0; i < 3; i++)
            {
                if (player.getInventory(i) == 0) {
                    player.setInventory(i, 5);
                    item->removeItem(map);
                }
            }
        }
        else {
            item->removeItem(map);
        }
        break;
    case 6:
        if (entity->getType() == 0) {
            for (int i = 0; i < 3; i++)
            {
                if (player.getInventory(i) == 0) {
                    player.setInventory(i, 6);
                    item->removeItem(map);
                }
            }
        }
        else {
            item->removeItem(map);
        }
        break;
    default:
        break;
    }
}

void resetScore()
{
    if (g_dElapsedTime > 2.0) // wait for 2 seconds to switch to menu mode, else do nothing
        g_eGameState = S_MAINMENU;
}

void titleWait()
{
    if (g_dElapsedTime > 2.0) // wait for 2 seconds to switch to menu mode, else do nothing
        g_eGameState = S_MAINMENU;
}

void updateTutorial(double dt)
{

}

void updateGame(double dt)       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    player.movement(map, g_skKeyEvent); // moves the character, collision detection, physics, etc

    if (g_skKeyEvent[K_SPACE].keyReleased && spamIncrease < 49)
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
        entityList.push_back(copPtr);
        ++copCount;
    }
    if (customerCount < 9)
    {
        Entity* customerPtr = new Customer;
        checkLocation(map, customerPtr);
        Customer* customer = dynamic_cast<Customer*>(customerPtr);
        customer->setPlayer(playerPtr);
        entityList.push_back(customerPtr);
        ++customerCount;
    }
    if (hoarderCount < 1)
    {
        Entity* hoarderPtr = new Hoarder;
        checkLocation(map, hoarderPtr);
        Hoarder* hoarder = dynamic_cast<Hoarder*>(hoarderPtr);
        hoarder->createPath(map);
        hoarder->solveAStar(map);
        entityList.push_back(hoarderPtr);
        ++hoarderCount;
    }
    if (itemCount < 4)
    {
        bool hasTP = false;
        for (int i = 0; i < itemCount; i++) // check if there is any toilet paper
        {
            if (itemPtr[i]->getItemType() == 1)
                hasTP = true;
        }
        if (hasTP)
            itemPtr[itemCount] = new Item();
        else
            itemPtr[itemCount] = new Item(1);
        checkItem(map, itemPtr[itemCount]);
        ++itemCount;
    }
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        if (entity->getType() == Entity::TYPE_HOARDER)
        {
            Hoarder* hoarder = dynamic_cast<Hoarder*>(entity);
            hoarder->movement(map, dt);
        }
        else if (entity->getType() != Entity::TYPE_COP)
            entity->move(map, dt);
        if (entity->getType() == Entity::TYPE_CHAD)
        {
            Chad* chad = dynamic_cast<Chad*>(entity);
            if (chad->checkCollision())
                chadPush();
        }
        if (entity->getType() == Entity::TYPE_CUSTOMER)
        {
            Customer* customer = dynamic_cast<Customer*>(entity);
            if (customer->checkCollision())
                customerBlock();
        }
        for (int i = 0; i < itemCount; i++)
        {
            if (entity->getPos('x') == itemPtr[i]->getPos('x') && entity->getPos('y') == itemPtr[i]->getPos('y'))
                pickedUpItem(map, itemPtr[i], entity, player);
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
    if (g_dElapsedTime > 5.0) // wait for 5 seconds to switch to main menu, else do nothing
        g_eGameState = S_MAINMENU;
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;    
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
        break;
    case S_GAMEOVER: renderGameOver();
        break;
    }
    renderHUD();      // renders debug information, frame rate, elapsed time, etc
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
        high_score = 0;
        
        g_eMenuState = S_OPTION1;
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
    for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
    {
        Entity* entity = (Entity*)*it;
        renderNPC(entity);
    }
    for (int i = 0; i < itemCount; i++)
    {
        renderItem(itemPtr[i]);
    }
    renderBar();
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
        ss << "Score: ";
        g_Console.writeToBuffer(t, ss.str());
    }
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6,
        00
    };

    COORD c;
    /*for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }*/
    //Map map;
    for (int C = 0; C < 24; C++)
    {
        c.Y = C+1;
        for (int R = 0; R < 80; R++)
        {
            c.X = R;
            if (map.getEntity(R, C) == 'w')
                g_Console.writeToBuffer(c, (char)219, colors[4]);
            else if (map.getEntity(R, C) == ' ')
                g_Console.writeToBuffer(c, (char)32, colors[12]);
            else
                g_Console.writeToBuffer(c, 'n', colors[12]);
        }
    }
}

void renderCharacter()
{
    COORD temp;
    temp.X = playerPtr->getPos('x');
    temp.Y = playerPtr->getPos('y');
    // Draw the location of the character
    playerPtr->setCharColor(0x0A);
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
    }
}

void renderItem(Item* item)
{
    COORD temp;
    temp.X = item->getPos('x');
    temp.Y = item->getPos('y');
    switch (item->getItemType())
    {
    case 1:
        g_Console.writeToBuffer(temp, (char)8, item->getCharColor());
        break;
    case 2:
        g_Console.writeToBuffer(temp, (char)22, item->getCharColor());
        break;
    case 3:
        g_Console.writeToBuffer(temp, (char)43, item->getCharColor());
        break;
    case 4:
        g_Console.writeToBuffer(temp, (char)127, item->getCharColor());
        break;
    case 5:
        g_Console.writeToBuffer(temp, (char)13, item->getCharColor());
        break;
    case 6:
        g_Console.writeToBuffer(temp, (char)7, item->getCharColor());
        break;
    }
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

    // displays inventory
    ss.str("");
    for (int i = 0; i < 3; i++) // TODO: find an ascii char to represent each item.
    {
        int itemid = player.getInventory(i);
        if (i == 0)
            ss << '|';
        if (itemid == 0)
            ss << ' ';
        else if (itemid == 1)
            ss << ' ';
        else if (itemid == 2)
            ss << ' ';
        else if (itemid == 3)
            ss << ' ';
        else if (itemid == 4)
            ss << ' ';
        else if (itemid == 5)
            ss << ' ';
        else if (itemid == 6)
            ss << ' ';
        if (i != 3)
            ss << '|';
    }
    c.X = 55;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays high score
    ss.str("");
    ss << "High score:" << high_score;
    c.X = 15;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    /*ss.str(""); TODO
    ss << "Current score:" << current_score;
    c.X = 15;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());*/
}

void renderBar()
{
    COORD pos;
    for (int i = 0; i < 20; ++i)
    {
        pos.X = 30 + i;
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
        case K_J: key = "Item slot 1";
            break;
        case K_K: key = "Item slot 2";
            break;
        case K_L: key = "Item slot 3";
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
    if (playerPtr->getPos('x') + 5 < g_Console.getConsoleSize().X &&
        playerPtr->getPos('y') + 5 < g_Console.getConsoleSize().Y &&
        playerPtr->getPos('x') - 5 > 0 &&
        playerPtr->getPos('y') - 5 > 0) // pushes the player
    {
        if (player.getDirection() == 0)
        {
            playerPtr->setPos('y', playerPtr->getPos('y') + 3);
        }
        else if (player.getDirection() == 1)
        {
            playerPtr->setPos('x', playerPtr->getPos('x') + 4);
        }
        else if (player.getDirection() == 2)
        {
            playerPtr->setPos('y', playerPtr->getPos('y') - 3);
        }
        else if (player.getDirection() == 3)
        {
            playerPtr->setPos('x', playerPtr->getPos('x') - 4);
        }
    }
}

void customerBlock()
{
    if (player.getDirection() == 0)
        playerPtr->setPos('y', playerPtr->getPos('y') + 1);
    else if (player.getDirection() == 1)
        playerPtr->setPos('x', playerPtr->getPos('x') + 1);
    else if (player.getDirection() == 2)
        playerPtr->setPos('y', playerPtr->getPos('y') - 1);
    else if (player.getDirection() == 3)
        playerPtr->setPos('x', playerPtr->getPos('x') - 1);
}

void checkLocation(Map map, Entity* entity)
{
    while (map.getEntity(entity->getPos('x'), entity->getPos('y') - 1) != ' ')
    {
        entity->reLoc();
    }
}

void checkItem(Map map, Item* item)
{
    while (map.getEntity(item->getPos('x'), item->getPos('y') - 1) != ' ')
    {
        item->reLoc();
    }
}