#include "Player.h"
#include "game.h"

//SKeyEvent g_skKeyEvent[K_COUNT];
//SGameChar g_sChar;

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
}

Player::~Player()
{
}

void Player::move()
{
	//if (g_skKeyEvent[0].keyDown && pos.Y > 0)
	//	pos.Y--;
	//if (g_skKeyEvent[2].keyDown && pos.X > 0)
	//	pos.X--;
	//if (g_skKeyEvent[1].keyDown /*&& pos.Y < g_Console.getConsoleSize().Y - 1*/)
	//	pos.Y++;
	//if (g_skKeyEvent[3].keyDown /*&& pos.X < g_Console.getConsoleSize().X - 1*/)
	//	pos.X++;
	////if (g_skKeyEvent[K_SPACE].keyReleased)

}
