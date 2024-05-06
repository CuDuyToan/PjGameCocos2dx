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

	CCLOG("player");

    auto keyboardListener = EventListenerKeyboard::create();

    // Gán các hàm xử lý sự kiện cho đối tượng EventListenerKeyboard
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);

    // Đăng ký đối tượng EventListenerKeyboard với trình quản lý sự kiện của Layer
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    this->schedule(CC_SCHEDULE_SELECTOR(Player::updateAction));

	return true;
}

void Player::createSprite()
{
	spritePlayer = Sprite::create("imageGura/GuraMeme (160).png");
	spritePlayer->setPosition(Vec2(400, 400));

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

<<<<<<< HEAD
void Player::playerPause()
{
}

=======
void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
   
    CCLOG("keyPress");
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_D:
        CCLOG("D");
        x = 200;
        setMoveR();

        break;
    case EventKeyboard::KeyCode::KEY_A:
        CCLOG("A");
        x = -200;
        setMoveL();
        break;
    case EventKeyboard::KeyCode::KEY_W:
        CCLOG("W");
        if (true)
        {
            y = 180;
            setMoveD();
        }
        break;
    case EventKeyboard::KeyCode::KEY_S:
        CCLOG("S");
        if (true)
        {
            y = -180;
            setMoveD();
        }

        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        CCLOG("Space bar");
        if (checkJump() == false)
        {
            force = 450;
            Jump();
        }

        break;
    //case EventKeyboard::KeyCode::KEY_ESCAPE:
    //    //backToSelectLevelScene();
    //    break;
    default:
        break;
    }
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    // Xử lý sự kiện khi một phím được thả ra
    CCLOG("keyReleased");
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_D:
        CCLOG("D");
        if (x == 200)
        {
            x = 0;
            setMoveIdle();
        }
        break;
    case EventKeyboard::KeyCode::KEY_A:
        CCLOG("A");
        if (x == -200)
        {
            x = 0;
            setMoveIdle();
        }
        break;
    case EventKeyboard::KeyCode::KEY_W:
        CCLOG("W");
        y = 0;
        //player->setMoveIdle();
        break;
    case EventKeyboard::KeyCode::KEY_S:
        CCLOG("S");
        y = 0;
        //player->setMoveIdle();
        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        CCLOG("Space bar");

        break;

    default:
        break;
    }
}

void Player::updateAction(float dt) {

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // Lấy vật lý thể hiện của sprite
    auto physicsBody = getPhysicsBody();

    if (checkMovePlayer() == 1)
    {

    }

    if (!isCollidingWall) {
        physicsBody->setVelocity(Vec2(x, physicsBody->getVelocity().y));
    }
    if (checkJump() == true)
    {
        physicsBody->setVelocity(Vec2(physicsBody->getVelocity().x, physicsBody->getVelocity().y + force));
        force = force - (force * 3 / 4);
        if (force <= 10)
        {
            force = 0;
        }
        //CCLOG("%d", force);

    }
}
>>>>>>> 7d15a36b18b3b5eb8f31af2167f9718d4cb4f4ac
