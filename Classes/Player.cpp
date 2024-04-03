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


	addSpriteFrames();

	auto moveAction = MoveBy::create(2.0f, Vec2(200, 0));

	//spritePlayer->runAction(moveAction);
	//createAnimation("GuraWalkVeryBasic(", 3, 0.07f);

	//createSprite();




	CCLOG("player");

	return true;
}

// chua dung
void Player::createSprite()
{
	spritePlayer = Sprite::create("imageGura/GuraMeme (160).png");
	spritePlayer->setPosition(Vec2(400, 400));

	// Thêm sprite từ OtherCppFile.cpp vào layer
	this->addChild(spritePlayer);

	CCLOG("sprite");
}

Animation* Player::createAnimation(std::string tenFrame, int soFrame, float delay)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/AhhShark.plist",
		"GuraPlayer/AhhShark.png");

	CCLOG("create Animation");

	Vector<SpriteFrame*> frames;
	for (int i = 1; i <= soFrame; ++i) {
		char buffer[20] = { 0 };
		CCLOG(buffer, "%d.png", i);
		std::string str = tenFrame + buffer;
		std::string frameName = tenFrame + std::to_string(i) + ").png";
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		if (frame) {
			frames.pushBack(frame);
		}
	}

	Animation* animation = Animation::createWithSpriteFrames(frames, delay);

	return animation;
}

void Player::addSpriteFrames()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraAction/GuraAllAction.plist",
		"GuraAction/GuraAllAction.png");

	spritePlayer = Sprite::createWithSpriteFrameName("GuraIdle (1).png");
	spritePlayer->setScale(1);
	this->addChild(spritePlayer);

	addPhysicBodyForSprite();

	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraIdle (", 4, 0.5f));
	animatePlayer->setTag(101);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));

}

//void Player::addSpriteFrames2()
//{
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/GuraWalkVeryBasic.plist",
//		"GuraPlayer/GuraWalkVeryBasic.png");
//
//	spritePlayer = Sprite::createWithSpriteFrameName("GuraWalkVeryBasic (1).png");
//	spritePlayer->setScale(1);
//	this->addChild(spritePlayer);
//
//	addPhysicBodyForSprite();
//
//	animatePlayer = Animate::create(Player::createAnimation("GuraWalkVeryBasic (", 10, 0.07f));
//	animatePlayer->setTag(1);
//	spritePlayer->runAction(RepeatForever::create(animatePlayer));
//
//	
//
//}

void Player::addPhysicBodyForSprite()
{
	physicPlayer = PhysicsBody::createBox(Size(
		spritePlayer->getContentSize().width / 5 * 4, spritePlayer->getContentSize().height) / 10 * 7,
		PhysicsMaterial(1.0f, 1.0f, 0.0f),
		Vec2(0, -(spritePlayer->getContentSize().height / 10)));
	physicPlayer->setDynamic(true);
	physicPlayer->setGravityEnable(true);


	physicPlayer->setContactTestBitmask(true);
	physicPlayer->setCollisionBitmask(100);

	this->addComponent(physicPlayer);
}


int Player::checkMovePlayer()
{
	return this->moveCheck;
}

bool Player::checkJump()
{
	return this->jumpCheck;
}

void Player::Jump()
{
	jumpCheck = true;
}

void Player::Grounding()
{
	jumpCheck = false;
}

void Player::setMoveL()
{
	spritePlayer->stopAllActions();
	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraWalkV1 (", 15, 0.025f));
	animatePlayer->setTag(201);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));
	spritePlayer->setFlippedX(true);
	moveCheck = -1;
}

void Player::setMoveR()
{
	spritePlayer->stopAllActions();
	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraWalkV1 (", 15, 0.025f));
	animatePlayer->setTag(201);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));
	spritePlayer->setFlippedX(false);
	moveCheck = 1;
}

void Player::setMoveU()
{
	moveCheck = 2;
}

void Player::setMoveD()
{
	moveCheck = -2;
}

void Player::setMoveIdle()
{
	spritePlayer->stopAllActions();
	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraIdle (", 4, 0.5f));
	animatePlayer->setTag(201);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));
	moveCheck = 0;
}

