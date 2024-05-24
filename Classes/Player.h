#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
//#include "GameScene.h"

using namespace cocos2d;

class Player :public Node
{
public:
	static Player* createPlayer();
	virtual bool init();

	Sprite* spritePlayer;

	Sprite* chat;

	PhysicsBody* physicPlayer;

	Animation* createAnimation(std::string tenFrame, int soFrame, float delay);

	void addSpriteFrames();
	void createSprite();
	void addPhysicBodyForSprite();

	void setMoveL();
	void setMoveR();
	void setMoveIdle();

	CREATE_FUNC(Player);
private:

	int contactCheck = 0;
	int inertia = 0;
	int inertiaMax = 9; 
};

#endif// _PLAYER_H_