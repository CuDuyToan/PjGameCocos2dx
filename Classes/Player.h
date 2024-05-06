#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

class Player :public Node
{
public:
	static Player* createPlayer();
	virtual bool init();

	Sprite* spritePlayer;
	//Animate* animatePlayer;

	PhysicsBody* physicPlayer;

	Animation* createAnimation(std::string tenFrame, int soFrame, float delay);

	void addPlayerToTiled();
	void addSpriteFrames();
	void addSpriteFrames2();
	void createSprite();
	void addPhysicBodyForSprite();

	//action
	void playerPause();

	int checkMovePlayer();
	bool checkJump();

	void setMoveL();
	void setMoveR();
	void setMoveU();
	void setMoveD();
	void setMoveIdle();
	void Jump();
	void Grounding();
	//

	CREATE_FUNC(Player);
private:
	bool moveLeft = false;
	bool moveRight = false;
	int moveCheck = 0;
	bool jumpCheck = true;
	int contactCheck = 0;
	int inertia = 0;
	int inertiaMax = 9; // lấy 3/4 chiều rộng sprite *9 ra tốc độ tối đa
};

#endif// _PLAYER_H_