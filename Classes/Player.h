// Player.h
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

using namespace cocos2d;

class Player : public cocos2d::Node
{
public:
    static Player* createPlayer();
    virtual bool init();
    CREATE_FUNC(Player);

    std::string items[5] = {};

    void setMoveL();
    void setMoveR();
    void setMoveIdle();
    void setWork();
    bool checkCanMove();

private:
    Sprite* spritePlayer;
    PhysicsBody* physicPlayer;
    bool canMove;
    int contactCheck;
    int inertia;
    int inertiaMax;

    void createSprite();
    void addSpriteFrames();
    void addPhysicBodyForSprite();
    Animation* createAnimation(std::string tenFrame, int soFrame, float delay);
    void changeCanMove(float dt);
};

#endif // _PLAYER_H_
