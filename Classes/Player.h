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

	bool isCollidingWall = false;

	float x = 0;
	float y = 0;
	float force = 0;

	Sprite* spritePlayer;
	//Animate* animatePlayer;

	PhysicsBody* physicPlayer;

	//GameScene* gameScene;

	Animation* createAnimation(std::string tenFrame, int soFrame, float delay);

	void addPlayerToTiled();
	void addSpriteFrames();
	void addSpriteFrames2();
	void createSprite();
	void addPhysicBodyForSprite();

<<<<<<< HEAD
	//action
	void playerPause();

=======
>>>>>>> 7d15a36b18b3b5eb8f31af2167f9718d4cb4f4ac
	int checkMovePlayer();
	bool checkJump();

	void setMoveL();
	void setMoveR();
	void setMoveU();
	void setMoveD();
	void setMoveIdle();
	void Jump();
	void Grounding();
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void updateAction(float dt);

	CREATE_FUNC(Player);
private:

	bool moveLeft = false;
	bool moveRight = false;
	int moveCheck = 0;
	bool jumpCheck = true;
	int contactCheck = 0;
	int inertia = 0;
	int inertiaMax = 9; 
};

#endif// _PLAYER_H_