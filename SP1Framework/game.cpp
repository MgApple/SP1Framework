// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "Map.h"
#include "Player.h"
#include "Chad.h"
#include "Cop.h"
#include "Customer.h"
#include "Hoarder.h"

std::string save;
int high_score;

double  g_dElapsedTime;
double  g_dDeltaTime;
double  g_dPrevPlayerTime;
double  g_dPrevChadTime;
double  g_dPrevCustomerTime;
double  g_dCooldown;
SKeyEvent g_skKeyEvent[K_COUNT];
//SMouseEvent g_mouseEvent;

// Game specific variables here HELP
Entity*      chadPtr;
Entity*      customerPtr;
Entity*      hoarderPtr;
Entity*      playerPtr;
Player*      player;
Chad         chad;
Cop          cop;
Customer     customer;
Hoarder      hoarder;
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_TITLE; // initial state s
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
    // Set precision for floating point output
    g_dElapsedTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_TITLE;

    playerPtr = new Player; 
    player = dynamic_cast<Player*>(playerPtr);
    playerPtr->setPos('x', g_Console.getConsoleSize().X / 2);
    playerPtr->setPos('y', g_Console.getConsoleSize().Y / 2);

    chadPtr = &chad;
    chad.setPlayer(playerPtr);
    customerPtr = &customer;
    customer.setPlayer(playerPtr);
    hoarderPtr = &hoarder;

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
    case S_MAINMENU: // don't handle anything for the splash screen
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
        case S_TITLE: Titlewait();
            break;
        case S_MAINMENU: splashScreenWait(); //temp thing until we can get menu buttons to work
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
        case S_GAMEOVER: gameOverWait(); // game logic for the gameover screen?
            break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 5.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void Titlewait()
{
    if (g_dElapsedTime > 2.0) // wait for 2 seconds to switch to menu mode, else do nothing
        g_eGameState = S_MAINMENU;
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    player->movement(map, g_skKeyEvent); // moves the character, collision detection, physics, etc

    double coolDown = g_dElapsedTime - g_dCooldown;
    if (player->getSpeed() && coolDown > 5.0f)
    {
        player->setSpeed(false);
        g_dCooldown = g_dElapsedTime;
    }
    double time = g_dElapsedTime - g_dPrevChadTime;
    if (time > 0.4f)
    {
        chadPtr->move(map);
        hoarderPtr->move(map);
        g_dPrevChadTime = g_dElapsedTime;
    }
    double time1 = g_dElapsedTime - g_dPrevCustomerTime;
    if (time1 > 0.4f)
    {
        customerPtr->move(map);
        g_dPrevCustomerTime = g_dElapsedTime;
    }
                                        //chad->move();
    chadPush(); // checks if chad pushes player
    //customer->move();
    customerBlock();
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
    c.X = 5;
    c.Y = 2;
    std::ifstream menu;
    std::string line;
    menu.open("menu.txt");
    if (menu) {
        while (getline(menu, line)) {
            g_Console.writeToBuffer(c, line);
            c.Y += 1;
        }
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
    renderNPC();
}

void renderGameOver()
{
    std::cout << "                                       .-:::::.`         `-----`      .------    `------`  .------------`" << "\n"
        << "                                    .omNNMMMMNNms.       yNNNNNy      +NNNNNN:   oNNNNNN.  yNNNNNNNNNNNN/" << "\n"
        << "                                   /NMMMNysshNMMMm.     +MMMNMMM+     oMMMNMMh  `NMMNMMM.  hMMMMhyyyyyyy-" << "\n"
        << "                                  .NMMMm.    -:-..     -NMMN:NMMN-    oMMMyNMM: sMMddMMM.  hMMMM+::::::-" << "\n"
        << "                                  /MMMMo   hddddddo   `mMMM+ oMMMm`   oMMMosMMd.NMM:dMMM.  hMMMMMMMMMMMd" << "\n"
        << "                                  :MMMMs   hddNMMMy   yMMMN///MMMMy   oMMMo.NMMdMMd dMMM.  hMMMMo++++++/" << "\n"
        << "                                   dMMMNo-.--/dMMMy  +MMMMMMMMMMMMM+  oMMMo sMMMMM: dMMM.  hMMMM/-------." << "\n"
        << "                                   `sNMMMNNNNMMMMd/ -NMMMh+++++dMMMN- oMMMo .NMMMd  dMMM.  hMMMMNNNNNNNNs" << "\n"
        << "                                     .+shdddhhs+-`  ohhhh.     -hhhhs /hhh/  ohhh:  shhh.  ohhhhhhhhhhhh+" << "\n"
        << "\n" << "\n" << "\n" << "                                  Personal Best: "<<high_score << "\n"
        <<"                                  Score: " << "\n" << "\n" << "\n"
        << "                                     `:+syhyyo/-    +ssss.     `ssss+ -ssssssssssss/  :sssssssssso/." << "\n"
        << "                                   `omMMMMNMMMMNh:  :NMMMh     sMMMN: +MMMMMMMMMMMMy  +MMMMNmmmMMMMm:" << "\n"
        << "                                   hMMMMs:--+mMMMN:  +MMMM/   -MMMMo  +MMMMs:::::::.  +MMMMo.../MMMMh" << "\n"
        << "                                  :MMMMy     .MMMMd   yMMMm`  dMMMh   +MMMMdyyyyyyy`  +MMMMy//+hMMMN/" << "\n"
        << "                                  +MMMM+      MMMMm   `mMMMs +MMMm.   +MMMMNmmmmmmm`  +MMMMNNMMMMms-" << "\n"
        << "                                  -MMMMh`    :MMMMy    :NMMM/NMMM:    +MMMMo.......   +MMMMs-sMMMNh-" << "\n"
        << "                                   oNMMMdo++yNMMMm.     +MMMNMMMo     +MMMMhooooooo+  +MMMM+  +NMMMm-" << "\n"
        << "                                    :hmNMMMMMMNdo`       hMMMMMd`     +MMMMMMMMMMMMN  +MMMM+   /NMMMN:" << "\n"
        << "                                      .-//++/:-`         `/////.      .////////////:  .////.    -////:" << "\n";
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
    map.loadMap();
    for (int R = 0; R < 24; R++)
    {
        c.Y = R+1;
        for (int C = 0; C < 80; C++)
        {
            c.X = C;
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
    if(chad.checkCollision())
        playerPtr->setCharColor(chadPtr->getCharColor());
    g_Console.writeToBuffer(temp, (char)21, playerPtr->getCharColor());
}

void renderNPC()
{
    COORD temp;
    temp.X = chad.getPos('x');
    temp.Y = chad.getPos('y');
    g_Console.writeToBuffer(temp, (char)4, chadPtr->getCharColor());
    temp.X = cop.getPos('x');
    temp.Y = cop.getPos('y');
    g_Console.writeToBuffer(temp, 'P', cop.getCharColor());
    temp.X = customer.getPos('x');
    temp.Y = customer.getPos('Y');
    g_Console.writeToBuffer(temp, 'C', customer.getCharColor());
    temp.X = hoarder.getPos('x');
    temp.Y = hoarder.getPos('y');
    g_Console.writeToBuffer(temp, 'H', hoarder.getCharColor());
}

void renderHUD()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());
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
    if (chad.checkCollision()) // pushes the player
    {
        // to be changed
        if (player->getDirection() == 0)
        {
            //playerPtr->setPos('x', playerPtr->getPos('x') + 4);
            playerPtr->setPos('y', playerPtr->getPos('y') + 3);
        }
        else if (player->getDirection() == 1)
        {
            playerPtr->setPos('x', playerPtr->getPos('x') + 4);
            //playerPtr->setPos('y', playerPtr->getPos('y') - 1);
        }
        else if (player->getDirection() == 2)
        {
            //playerPtr->setPos('x', playerPtr->getPos('x') + 4);
            playerPtr->setPos('y', playerPtr->getPos('y') - 3);
        }
        else if (player->getDirection() == 3)
        {
            playerPtr->setPos('x', playerPtr->getPos('x') - 4);
            //playerPtr->setPos('y', playerPtr->getPos('y') - 1);
        }
    }
}

void customerBlock()
{
    if (customer.checkCollision()) // pushes the player
    {
        // to be changed
        if (player->getDirection() == 0)
        {
            //player.setPos('x', player.getPos('x') + 4);
            playerPtr->setPos('y', playerPtr->getPos('y') + 1);
        }
        else if (player->getDirection() == 1)
        {
            playerPtr->setPos('x', playerPtr->getPos('x') + 1);
            //player.setPos('y', player.getPos('y') - 1);
        }
        else if (player->getDirection() == 2)
        {
            //player.setPos('x', player.getPos('x') + 4);
            playerPtr->setPos('y', player->getPos('y') - 1);
        }
        else if (player->getDirection() == 3)
        {
            player->setPos('x', player->getPos('x') - 1);
            //player.setPos('y', player.getPos('y') - 1);
        }
    }
}