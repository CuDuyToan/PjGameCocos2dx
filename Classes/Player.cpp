// Player.cpp
#include "Player.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "MenuLayer.h"

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

    //createSprite();
    addSpriteFrames();
    addPhysicBodyForSprite();
    action = true;
    contactCheck = 0;
    inertia = 0;
    inertiaMax = 9;

    return true;
}

void Player::createSprite()
{
    spritePlayer = Sprite::create("imageGura/GuraMeme (160).png");
    spritePlayer->setPosition(Vec2(400, 400));
    this->addChild(spritePlayer);
}

Animation* Player::createAnimation(std::string tenFrame, int soFrame, float delay)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/AhhShark.plist",
        "GuraPlayer/AhhShark.png");

    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= soFrame; ++i) {
        std::string frameName = tenFrame + std::to_string(i) + ").png";
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
        }
    }
    return Animation::createWithSpriteFrames(frames, delay);
}

void Player::addSpriteFrames()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraAction/GuraAllAction.plist",
        "GuraAction/GuraAllAction.png");

    spritePlayer = Sprite::createWithSpriteFrameName("GuraIdle (1).png");
    spritePlayer->setScale(1);
    this->addChild(spritePlayer);

    Animate* animatePlayer = Animate::create(createAnimation("GuraIdle (", 4, 0.5f));
    animatePlayer->setTag(101);
    spritePlayer->runAction(RepeatForever::create(animatePlayer));
}

void Player::addPhysicBodyForSprite()
{
    physicPlayer = PhysicsBody::createBox(Size(spritePlayer->getContentSize().width / 5 * 4,
        spritePlayer->getContentSize().height) / 10 * 7,
        PhysicsMaterial(1.0f, 1.0f, 0.0f),
        Vec2(0, -(spritePlayer->getContentSize().height / 15)));
    physicPlayer->setDynamic(true);
    physicPlayer->setGravityEnable(true);
    physicPlayer->setRotationEnable(false);
    physicPlayer->setContactTestBitmask(true);
    physicPlayer->setCollisionBitmask(100);

    this->addComponent(physicPlayer);
}

void Player::setMoveL()
{
    spritePlayer->stopAllActions();
    Animate* animatePlayer = Animate::create(createAnimation("GuraWalkV1 (", 15, 0.025f));
    animatePlayer->setTag(201);
    spritePlayer->runAction(RepeatForever::create(animatePlayer));
    spritePlayer->setFlippedX(true);
}

void Player::setMoveR()
{
    spritePlayer->stopAllActions();
    Animate* animatePlayer = Animate::create(createAnimation("GuraWalkV1 (", 15, 0.025f));
    animatePlayer->setTag(201);
    spritePlayer->runAction(RepeatForever::create(animatePlayer));
    spritePlayer->setFlippedX(false);
}

void Player::setMoveIdle()
{
    spritePlayer->stopAllActions();
    Animate* animatePlayer = Animate::create(createAnimation("GuraIdle (", 4, 0.5f));
    animatePlayer->setTag(201);
    spritePlayer->runAction(RepeatForever::create(animatePlayer));
}

void Player::setWork()
{
    action = false;
    if (!action)
    {
        spritePlayer->stopAllActions();
        Animate* animatePlayer = Animate::create(createAnimation("GuraWork (", 10, 0.2f));
        animatePlayer->setTag(201);
        spritePlayer->runAction(RepeatForever::create(animatePlayer));
        this->scheduleOnce(CC_CALLBACK_1(Player::changeActionStatus, this), 1.0f, "change_can_move_key");
    }
}

void Player::changeActionStatus(float dt)
{
    action = !action;
    if (action)
    {
        setMoveIdle();
    }
}

float Player::getSizePlayer()
{
    auto defaultSprite = Sprite::create("imageGura/GuraMeme (160).png");
    return  defaultSprite->getContentSize().height;
}

bool Player::checkAction()
{
    return action;
}

