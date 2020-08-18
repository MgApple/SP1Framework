#include "Player.h"

Player::Player() : Entity(TYPE::TYPE_PLAYER)
{
	for (int i = 0; i < 3; i++)
	{
		inventory[i] = 0;
	}
	stamina = 100;
	speedBuff = false;
	securityPass = false;
	alarmClock = false;
	isActive = false;
	key = new SKeyEvent(); 
	WORD charColor = 0x0C;
}

Player::~Player()
{
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

void Player::move()
{
	if (key[0].keyDown && pos.Y > 0)
		pos.Y--;
	if (key[2].keyDown && pos.X > 0)
		pos.X--;
	if (key[1].keyDown/*&& pos.Y < g_Console.getConsoleSize().Y - 1*/)
		pos.Y++;
	if (key[3].keyDown/*&& pos.X < g_Console.getConsoleSize().X - 1*/)
		pos.X++;
	//if (g_skKeyEvent[K_SPACE].keyReleased)

}

void Player::render()
{
	// Draw the location of the character
	if (isActive)
	{
		charColor = 0x0A;
	}
}