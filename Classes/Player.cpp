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

	//CCLOG("player");

	/*auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	this->schedule(CC_SCHEDULE_SELECTOR(Player::updateAction));*/

	/*auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(Player::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(CC_SCHEDULE_SELECTOR(Player::updateAction));*/

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Player::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	chat = Sprite::create("res/chat.png");
	chat->setScale(0.3);
	chat->setPosition(Vec2(spritePlayer->getContentSize().width - 50, spritePlayer->getContentSize().height - 50));
	chat->setVisible(false);
	this->addChild(chat);


	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Player::onPhysicsContactBegin, this);
	listener->onContactSeparate = CC_CALLBACK_1(Player::onPhysicsContactEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void Player::createSprite()
{
	spritePlayer = Sprite::create("imageGura/GuraMeme (160).png");
	spritePlayer->setPosition(Vec2(400, 400));

	this->addChild(spritePlayer);

	//CCLOG("sprite");
}

Animation* Player::createAnimation(std::string tenFrame, int soFrame, float delay)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/AhhShark.plist",
		"GuraPlayer/AhhShark.png");

	//CCLOG("create Animation");

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

void Player::addPhysicBodyForSprite()
{
	physicPlayer = PhysicsBody::createBox(Size(
		spritePlayer->getContentSize().width / 5 * 4, spritePlayer->getContentSize().height) / 10 * 7,
		PhysicsMaterial(1.0f, 1.0f, 0.0f),
		Vec2(0, -(spritePlayer->getContentSize().height / 10)));
	physicPlayer->setDynamic(true);
	physicPlayer->setGravityEnable(true);
	physicPlayer->setRotationEnable(false);


	physicPlayer->setContactTestBitmask(true);
	physicPlayer->setCollisionBitmask(100);

	this->addComponent(physicPlayer);
}

//int Player::checkMovePlayer()
//{
//	//return this->moveCheck;
//}



//void Player::CheckMove()
//{
//	playerMove = false;
//}

void Player::setMoveL()
{
	//moveCheck = -1;
	spritePlayer->stopAllActions();
	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraWalkV1 (", 15, 0.025f));
	animatePlayer->setTag(201);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));
	spritePlayer->setFlippedX(true);
	//moveCheck = -1;
}

void Player::setMoveR()
{
	//moveCheck = 1;
	spritePlayer->stopAllActions();
	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraWalkV1 (", 15, 0.025f));
	animatePlayer->setTag(201);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));
	spritePlayer->setFlippedX(false);
	//moveCheck = 1;
}

void Player::setMoveU()
{
	//moveCheck = 2;
}

void Player::setMoveD()
{
	//moveCheck = -2;
}

void Player::setMoveIdle()
{
	spritePlayer->stopAllActions();
	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraIdle (", 4, 0.5f));
	animatePlayer->setTag(201);
	spritePlayer->runAction(RepeatForever::create(animatePlayer));
	//moveCheck = 0;
}

//void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
//    CCLOG("keyPress");
//    switch (keyCode)
//    {
//    case EventKeyboard::KeyCode::KEY_D:
//        CCLOG("D");
//        x = 200;
//        setMoveR();
//
//        break;
//    case EventKeyboard::KeyCode::KEY_A:
//        CCLOG("A");
//        x = -200;
//        setMoveL();
//        break;
//    case EventKeyboard::KeyCode::KEY_W:
//        CCLOG("W");
//        if (true)
//        {
//            y = 180;
//            setMoveD();
//        }
//        break;
//    case EventKeyboard::KeyCode::KEY_S:
//        CCLOG("S");
//        if (true)
//        {
//            y = -180;
//            setMoveD();
//        }
//
//        break;
//    case EventKeyboard::KeyCode::KEY_SPACE:
//        
//        if (checkJump() == false)
//        {
//            force = 450;
//            Jump();
//        }
//
//        break;
//    //case EventKeyboard::KeyCode::KEY_ESCAPE:
//    //    //backToSelectLevelScene();
//    //    break;
//    default:
//        break;
//    }
//}
//
//void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
//
//    CCLOG("keyReleased");
//    switch (keyCode)
//    {
//    case EventKeyboard::KeyCode::KEY_D:
//        if (x == 200)
//        {
//            x = 0;
//            setMoveIdle();
//        }
//        break;
//    case EventKeyboard::KeyCode::KEY_A:
//        
//        if (x == -200)
//        {
//            x = 0;
//            setMoveIdle();
//        }
//        break;
//    case EventKeyboard::KeyCode::KEY_W:
//        
//        y = 0;
//        //player->setMoveIdle();
//        break;
//    case EventKeyboard::KeyCode::KEY_S:
//        
//        y = 0;
//        //player->setMoveIdle();
//        break;
//    case EventKeyboard::KeyCode::KEY_SPACE:
//       
//        break;
//
//    default:
//        break;
//    }
//}

//void Player::updateAction(float dt) {
//
//    //auto physicsBody = getPhysicsBody();
//
//    if (checkMovePlayer() == 1)
//    {
//        x = 200;
//    }
//    else if (checkMovePlayer() == -1)
//    {
//        x = -200;
//    }
//
//    if (!isCollidingWall) {
//		physicPlayer->setVelocity(Vec2(x, physicPlayer->getVelocity().y));
//    }
//}

bool Player::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocationInView();

	auto touchPointInNode = this->getParent()->convertToNodeSpace(touchPoint);

	float deltaX = touchPointInNode.x - this->getPositionX();

	if (deltaX < 0)
	{
		this->setMoveL();
	}
	else
	{
		this->setMoveR();
	}

	float distanceX = fabs(deltaX);
	float duration = distanceX / 200.0f;

	this->stopAllActions();
	//this->runAction(MoveTo::create(duration, Vec2(touchPointInNode.x, this->getPositionY())));
	/*if (!isColliding)
	{*/
	this->runAction(Sequence::create(
		MoveTo::create(duration, Vec2(touchPointInNode.x, this->getPositionY())),
		CallFunc::create([this]() {

			this->setMoveIdle();
			}),
		nullptr
	));


	/*else
	{
		this->stopAllActions();
		this->setMoveIdle();
		isColliding = false;
	}*/

	return true;
}

bool Player::onPhysicsContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody();
	auto nodeB = contact.getShapeB()->getBody();

	if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 0 ||
		nodeA->getCollisionBitmask() == 0 && nodeB->getCollisionBitmask() == 100)
	{
		if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
		{
			if (nodeA->getTag() == 100 || nodeB->getTag() == 100)
			{
				CCLOG("accecpt");
			}

			if (nodeA->getCollisionBitmask() == 0)
			{
				chat->setVisible(true);
			}

			if (nodeB->getCollisionBitmask() == 0)
			{
				chat->setVisible(true);
			}
		}
	}

	return true;
}

bool Player::onPhysicsContactEnd(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody();
	auto nodeB = contact.getShapeB()->getBody();

	if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 0 ||
		nodeA->getCollisionBitmask() == 0 && nodeB->getCollisionBitmask() == 100)
	{
		if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
		{
			if (nodeA->getCollisionBitmask() == 0)
			{
				chat->setVisible(false);
			}

			if (nodeB->getCollisionBitmask() == 0)
			{
				chat->setVisible(false);
			}
		}
	}

	return true;
}
