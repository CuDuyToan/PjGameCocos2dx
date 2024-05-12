#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

using namespace cocos2d;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createPhysicsWorld();

    virtual bool init();

    Player* player;
    //PhysicsBody* physicPlayer;

    /*virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);*/

    bool createTileMap();

    void addSpriteToTileMap();
    void createBackGroundGame();

    void getGround();
    void getWall();
    void getCeiling();

    bool onContactBegin(PhysicsContact& contact);
    bool onContactSeparate(PhysicsContact& contact);

    //void updateAction(float);

    void backToSelectLevelScene();

    void createButtonGame();

    void pauseGame();

    void moveCamera(float);
    //void increaseGravity(int* grav);

    cocos2d::TMXTiledMap* _tilemap;


    CREATE_FUNC(GameScene);

private:
    /*int x = 0;
    int y = 0;
    int force = 0;
    bool isCollidingWall = false;*/
    PhysicsWorld* world;
    void setPhysicWorld(PhysicsWorld* _world)
    {
        world = _world;
    }
};

#endif // __GAME_SCENE_H__
