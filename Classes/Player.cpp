// Player.cpp
#include "Player.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "MenuLayer.h"
#include "AudioEngine.h"


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
    addChatDanger();

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
    physicPlayer->setName("player physic");

    this->addComponent(physicPlayer);
}

void Player::setMoveL()
{
    moveSoundEffect(true);
    spritePlayer->stopAllActions();
    Animate* animatePlayer = Animate::create(createAnimation("GuraWalkV1 (", 15, 0.025f));
    animatePlayer->setTag(201);
    spritePlayer->runAction(RepeatForever::create(animatePlayer));
    spritePlayer->setFlippedX(true);
}

void Player::setMoveR()
{
    moveSoundEffect(true);
    spritePlayer->stopAllActions();
    Animate* animatePlayer = Animate::create(createAnimation("GuraWalkV1 (", 15, 0.025f));
    animatePlayer->setTag(201);
    spritePlayer->runAction(RepeatForever::create(animatePlayer));
    spritePlayer->setFlippedX(false);
}

void Player::moveSoundEffect(bool trueFalse)
{
    std::string pathEffect = "inGame/sound/walk.mp3";

    if (moveEffectSound)
    {
        AudioEngine::stop(moveEffectSound);
        //CCLOG("first effect");
    }
    
    if (trueFalse)
    {

        moveEffectSound = cocos2d::AudioEngine::play2d(pathEffect, true);
        CCLOG("move effect");
        
    }else if (!trueFalse)
    {
        AudioEngine::stop(moveEffectSound);
        CCLOG("tat effect");
    }
    else
    {
        CCLOG("no move sound effect");
    }
}

void Player::setMoveIdle()
{
    moveSoundEffect(false);

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

float Player::getSizePlayerHeight()
{
    auto defaultSprite = Sprite::create("imageGura/GuraMeme (160).png");
    return  defaultSprite->getContentSize().height;
}

bool Player::checkAction()
{
    return action;
}

void Player::addChatDanger()
{
    auto chat = Sprite::create("inGame/chat icon/danger.png");
    chat->setScale(0.1*(Director::getInstance()->getVisibleSize().height / chat->getContentSize().height));
    chat->setVisible(false);
    chat->setName("danger");

    this->addChild(chat, 100);
    chat->setPosition(Vec2(0, spritePlayer->getContentSize().height*0.6));
}

void Player::dangerNotice(bool trueFalse)
{
    this->getChildByName("danger")->setVisible(true);

    this->scheduleOnce([=](float dt) mutable {
        this->getChildByName("danger")->setVisible(false);
        }, 1.0f, "unvisible danger");

    // Danh sách các tệp âm thanh
    std::vector<std::string> soundFiles = {
        "inGame/sound/ahh sound_effect.mp3",
        "inGame/sound/oh sound_effect.mp3",
    };

    // Tạo máy phát số ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, soundFiles.size() - 1);

    // Chọn ngẫu nhiên một tệp âm thanh từ danh sách
    int randomIndex = dis(gen);
    std::string randomSoundFile = soundFiles[randomIndex];

    // Phát âm thanh ngẫu nhiên
    int audioID = AudioEngine::play2d(randomSoundFile, false, 1.0f);

    // Đặt âm lượng ngẫu nhiên (nếu muốn)
    float randomVolume = static_cast<float>(dis(gen)) / (soundFiles.size() - 1);
    AudioEngine::setVolume(audioID, randomVolume);


    //this->getChildByName("danger")->setVisible(trueFalse);
    //std::string pathSound = "inGame/sound/ahh sound_effect.mp3";
    //if (trueFalse)
    //{
    //    AudioEngine::play2d(pathSound);
    //}
}

