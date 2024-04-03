#include "Player.h"

USING_NS_CC;

Player* Player::createPlayer()
{
	return Player::create();

}

bool Player::init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}