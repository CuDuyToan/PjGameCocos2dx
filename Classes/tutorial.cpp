// tutorial.cpp
#include "tutorial.h"


USING_NS_CC;

Tutorial* Tutorial::createTutorial()
{
    return Tutorial::create();
}

bool Tutorial::init()
{
    if (!Node::init())
    {
        return false;
    }
    addSpriteFrames();
    return true;
}

Animation* Tutorial::createAnimation(std::string tenFrame, int soFrame, float delay)
{
    //SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/AhhShark.plist",
    //    "GuraPlayer/AhhShark.png");

    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= soFrame; ++i) {
        std::string frameName = tenFrame + std::to_string(i) + ".png";
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
        }
    }
    return Animation::createWithSpriteFrames(frames, delay);
}

void Tutorial::addSpriteFrames()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("inGame/tutorial/tap asset.plist",
        "inGame/tutorial/tap asset.png");

    fingerTapAnimation = Sprite::createWithSpriteFrameName("tile000.png");
    fingerTapAnimation->setAnchorPoint(Vec2(0, 1));
    this->addChild(fingerTapAnimation);

    Animate* animatePlayer = Animate::create(createAnimation("tile00", 3, 0.5f));
    animatePlayer->setTag(101);
    fingerTapAnimation->runAction(RepeatForever::create(animatePlayer));
}

Size Tutorial::getSpriteSize()
{
    Size spriteSize = fingerTapAnimation->getContentSize();
    return spriteSize;
}

