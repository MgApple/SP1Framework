#include "Player.h"

Player::Player() : inventory{ 0 }
{
	type = TYPE_PLAYER;
	stamina = 100;
	hasSpeedBuff = false;
	hasStaminaBuff = false;
	hasSecurityPass = false;
	isActive = true;
	charColor = 0x0C;
	direction = NONE;
}

Player::~Player()
{
	// 
}

int Player::getInventory(int idx)
{
	return inventory[idx];
}

void Player::setInventory(int idx, int item)
{
	inventory[idx] = item;
}

bool Player::getPState(char att)
{
	if (att == 's')
		return hasStaminaBuff;
	else if (att == 'p')
		return hasSecurityPass;
	else
		return false;
}

void Player::setPState(char att, bool state)
{
	if (att == 's')
		hasStaminaBuff = state;
	else if (att == 'p')
		hasSecurityPass = state;
}

void Player::setActive(bool isActive)
{
	this->isActive = isActive;
}

bool Player::getActive()
{
	return isActive;
}

void Player::setSpeed(bool speedBuff)
{
	this->hasSpeedBuff = speedBuff;
}

bool Player::getSpeed()
{
	return hasSpeedBuff;
}

void Player::move(Map& map)
{
}

void Player::movement(Map& map, SKeyEvent* key)
{
	if (key[0].keyDown && pos.Y > 0)
	{
		if (hasSpeedBuff)
			pos.Y -= 2;
		else
			pos.Y--;
		direction = UP;
	}
	else if (key[2].keyDown && pos.X > 0)
	{
		if (hasSpeedBuff)
			pos.X -= 2;
		else
			pos.X--;
		direction = LEFT;
	}
	else if (key[1].keyDown && pos.Y < 25 - 1)
	{
		if (hasSpeedBuff)
			pos.Y += 2;
		else
			pos.Y++;
		direction = DOWN;
	}
	else if (key[3].keyDown && pos.X < 80 - 1)
	{
		if (hasSpeedBuff)
			pos.X += 2;
		else
			pos.X++;
		direction = RIGHT;
	}
	if (key[K_SHIFT].keyDown && pos.Y > 1)
		hasSpeedBuff = true;

	int wall = map.getEntity(pos.Y - 1, pos.X);
	if (wall == 'w')
	{
		if (direction == UP)
			pos.Y++;
		else if (direction == LEFT)
			pos.X++;
		else if (direction == DOWN)
			pos.Y--;
		else if (direction == RIGHT)
			pos.X--;
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