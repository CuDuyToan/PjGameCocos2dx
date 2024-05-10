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

	bool isColliding = false;

	float x = 0;
	float y = 0;
	float force = 0;

	Sprite* spritePlayer;

	Sprite* chat;

	bool playerMove = true;

	PhysicsBody* physicPlayer;

	//GameScene* gameScene;

	Animation* createAnimation(std::string tenFrame, int soFrame, float delay);

	void addPlayerToTiled();
	void addSpriteFrames();
	void addSpriteFrames2();
	void createSprite();
	void addPhysicBodyForSprite();


	void playerPause();
	int checkMovePlayer();
	bool checkJump();

	void setMoveL();
	void setMoveR();
	void setMoveU();
	void setMoveD();
	void setMoveIdle();
	void Jump();
	void CheckMove();
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	bool onPhysicsContactBegin(PhysicsContact& contact);

	bool onPhysicsContactEnd(PhysicsContact& contact);

	void updateAction(float dt);

	bool onTouchBegan(Touch* touch, Event* event);

	CREATE_FUNC(Player);
private:

	/*bool moveLeft = false;
	bool moveRight = false;
	int moveCheck = 0;*/
	//bool playerMove = true;
	int contactCheck = 0;
	int inertia = 0;
	int inertiaMax = 9; 
};

#endif// _PLAYER_H_