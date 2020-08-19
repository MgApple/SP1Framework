#include "Player.h"

Player::Player() : inventory{ 0 }
{
	type = TYPE_PLAYER;
	stamina = 100;
	speedBuff = false;
	securityPass = false;
	alarmClock = false;
	isActive = true;
	key = new SKeyEvent(); 
	charColor = 0x0C;
	direction = NONE;
}

Player::~Player()
{
	// 
}

void Player::setActive(bool isActive)
{
	this->isActive = isActive;
}

bool Player::getActive()
{
	return isActive;
}

void Player::setKey(SKeyEvent* g_skKeyEvent)
{
	key = g_skKeyEvent;
}

WORD Player::getCharColor()
{
	return charColor;
}

void Player::setCharColor(WORD charColor)
{
	this->charColor = charColor;
}

void Player::move(Map &map)
{
	if (key[0].keyDown && pos.Y > 0)
	{
		if (key[K_SHIFT].keyDown)
			pos.Y += 2;
		pos.Y--;
		direction = UP;
	}
	if (key[2].keyDown && pos.X > 0)
	{
		pos.X--;
		direction = LEFT;
	}
	if (key[1].keyDown/*&& pos.Y < g_Console.getConsoleSize().Y - 1*/)
	{
		pos.Y++;
		direction = DOWN;
	}
	if (key[3].keyDown/*&& pos.X < g_Console.getConsoleSize().X - 1*/)
	{
		pos.X++;
		direction = RIGHT;
	}
}

void Player::render()
{
	// Draw the location of the character
	if(isActive)
		charColor = 0x0A;
}

enum Player::DIRECTION Player::getDirection()
{
	return direction;
}

//void moveCharacter()
//{    
//    // Updating the location of the character based on the key release
//    // providing a beep sound whenver we shift the character
//    if (g_skKeyEvent[0].keyDown && g_sChar.m_cLocation.Y > 0)
//    {
//        //Beep(1440, 30);
//        g_sChar.m_cLocation.Y--;       
//    }
//
//    if (g_skKeyEvent[2].keyDown && g_sChar.m_cLocation.X > 0)
//    {
//        //Beep(1440, 30);
//        g_sChar.m_cLocation.X--;        
//    }
//    if (g_skKeyEvent[1].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
//    {
//        //Beep(1440, 30);
//        g_sChar.m_cLocation.Y++;        
//    }
//    if (g_skKeyEvent[3].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
//    {
//        //Beep(1440, 30);
//        g_sChar.m_cLocation.X++;        
//    }
//    if (g_skKeyEvent[K_SPACE].keyReleased)
//    {
//        g_sChar.m_bActive = !g_sChar.m_bActive;        
//    }
//}