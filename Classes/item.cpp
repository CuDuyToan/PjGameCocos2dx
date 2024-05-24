//#include "Player.h"
//#include "MenuScene.h"
//#include "GameScene.h"
//#include "MenuLayer.h"
//
//USING_NS_CC;
//
//Player* Player::createPlayer()
//{
//	return Player::create();
//
//}
//
//bool Player::init()
//{
//	if (!Node::init())
//	{
//		return false;
//	}
//
//	addSpriteFrames();
//
//	chat = Sprite::create("res/chat.png");
//	chat->setScale(0.3);
//	chat->setPosition(Vec2(spritePlayer->getContentSize().width - 50, spritePlayer->getContentSize().height - 50));
//	chat->setVisible(false);
//	this->addChild(chat);
//
//
//	auto listener = EventListenerPhysicsContact::create();
//	listener->onContactBegin = CC_CALLBACK_1(Player::onPhysicsContactBegin, this);
//	listener->onContactSeparate = CC_CALLBACK_1(Player::onPhysicsContactEnd, this);
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//
//
//	return true;
//}
//
//void Player::createSprite()
//{
//	spritePlayer = Sprite::create("imageGura/GuraMeme (160).png");
//	spritePlayer->setPosition(Vec2(400, 400));
//
//	this->addChild(spritePlayer);
//
//}
//
//Animation* Player::createAnimation(std::string tenFrame, int soFrame, float delay)
//{
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/AhhShark.plist",
//		"GuraPlayer/AhhShark.png");
//
//
//	Vector<SpriteFrame*> frames;
//	for (int i = 1; i <= soFrame; ++i) {
//		char buffer[20] = { 0 };
//		//CCLOG(buffer, "%d.png", i);
//		std::string str = tenFrame + buffer;
//		std::string frameName = tenFrame + std::to_string(i) + ").png";
//		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
//		if (frame) {
//			frames.pushBack(frame);
//		}
//	}
//	Animation* animation = Animation::createWithSpriteFrames(frames, delay);
//
//	return animation;
//}
//
//void Player::addSpriteFrames()
//{
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraAction/GuraAllAction.plist",
//		"GuraAction/GuraAllAction.png");
//
//	spritePlayer = Sprite::createWithSpriteFrameName("GuraIdle (1).png");
//	spritePlayer->setScale(1);
//	this->addChild(spritePlayer);
//
//	addPhysicBodyForSprite();
//
//	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraIdle (", 4, 0.5f));
//	animatePlayer->setTag(101);
//	spritePlayer->runAction(RepeatForever::create(animatePlayer));
//
//}
//
//void Player::addPhysicBodyForSprite()
//{
//	physicPlayer = PhysicsBody::createBox(Size(
//		spritePlayer->getContentSize().width / 5 * 4, spritePlayer->getContentSize().height) / 10 * 7,
//		PhysicsMaterial(1.0f, 1.0f, 0.0f),
//		Vec2(0, -(spritePlayer->getContentSize().height / 10)));
//	physicPlayer->setDynamic(true);
//	physicPlayer->setGravityEnable(true);
//	physicPlayer->setRotationEnable(false);
//
//
//	physicPlayer->setContactTestBitmask(true);
//	physicPlayer->setCollisionBitmask(100);
//
//	this->addComponent(physicPlayer);
//}
//
//void Player::setMoveL()
//{
//	//moveCheck = -1;
//	spritePlayer->stopAllActions();
//	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraWalkV1 (", 15, 0.025f));
//	animatePlayer->setTag(201);
//	spritePlayer->runAction(RepeatForever::create(animatePlayer));
//	spritePlayer->setFlippedX(true);
//	//moveCheck = -1;
//	//playerCanMove = false;
//}
//
//void Player::setMoveR()
//{
//	//moveCheck = 1;
//	spritePlayer->stopAllActions();
//	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraWalkV1 (", 15, 0.025f));
//	animatePlayer->setTag(201);
//	spritePlayer->runAction(RepeatForever::create(animatePlayer));
//	spritePlayer->setFlippedX(false);
//	//moveCheck = 1;
//	//playerCanMove = false;
//}
//
//bool Player::checkCanMove()
//{
//	return playerCanMove;
//}
//
//void Player::setMoveIdle()
//{
//	spritePlayer->stopAllActions();
//	Animate* animatePlayer = Animate::create(Player::createAnimation("GuraIdle (", 4, 0.5f));
//	animatePlayer->setTag(201);
//	spritePlayer->runAction(RepeatForever::create(animatePlayer));
//	//playerCanMove = true;
//}
//
//bool Player::onPhysicsContactBegin(PhysicsContact& contact)
//{
//	auto nodeA = contact.getShapeA()->getBody();
//	auto nodeB = contact.getShapeB()->getBody();
//
//	if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 0 ||
//		nodeA->getCollisionBitmask() == 0 && nodeB->getCollisionBitmask() == 100)
//	{
//		if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
//		{
//			if (nodeA->getTag() == 100 || nodeB->getTag() == 100)
//			{
//				CCLOG("accecpt");
//			}
//
//			if (nodeA->getCollisionBitmask() == 0)
//			{
//				chat->setVisible(true);
//			}
//
//			if (nodeB->getCollisionBitmask() == 0)
//			{
//				chat->setVisible(true);
//			}
//		}
//	}
//
//	return true;
//}
//
//bool Player::onPhysicsContactEnd(PhysicsContact& contact)
//{
//	auto nodeA = contact.getShapeA()->getBody();
//	auto nodeB = contact.getShapeB()->getBody();
//
//	if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 0 ||
//		nodeA->getCollisionBitmask() == 0 && nodeB->getCollisionBitmask() == 100)
//	{
//		if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
//		{
//			if (nodeA->getCollisionBitmask() == 0)
//			{
//				chat->setVisible(false);
//			}
//
//			if (nodeB->getCollisionBitmask() == 0)
//			{
//				chat->setVisible(false);
//			}
//		}
//	}
//
//	return true;
//}
