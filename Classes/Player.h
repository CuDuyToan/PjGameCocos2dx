#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

class Player :public Node
{
public:
	static Player* createPlayer();
	virtual bool init();



	Animation* createAnimation(std::string tenFrame, int soFrame, float delay);


	int getMoveCheck();
	void setMoveRight();
	void setMoveLeft();
	void setMoveIdle();
	void setJump();

	int getHealth();
	void setHealth(int);

	void updateHealth(float);

	CREATE_FUNC(Player);
private:
	int moveCheck =0;
	int healthShip = 5;
	int life = 0;
};

#endif// _PLAYER_H_