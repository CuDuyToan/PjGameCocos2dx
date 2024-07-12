// tutorial.h
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "cocos2d.h"

using namespace cocos2d;

class Tutorial : public cocos2d::Node
{
public:
    static Tutorial* createTutorial();
    virtual bool init();
    CREATE_FUNC(Tutorial);

    cocos2d::Size getSpriteSize();

private:
    Sprite* fingerTapAnimation;

    void addSpriteFrames();
    Animation* createAnimation(std::string tenFrame, int soFrame, float delay);
};

#endif // _PLAYER_H_
