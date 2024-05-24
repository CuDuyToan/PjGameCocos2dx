#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "MenuLayer.h"

using namespace cocos2d;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createPhysicsWorld();

    virtual bool init();

    Player* player;

    bool createTileMap();

    void getGround();
    void getWall();
    void getCeiling();
    void getBarrier();
    void getAllQuest();
    void getDoor();
    void getHideItem();

    void getQuestList();

    void getChildOfTileMapWithName(std::string name);
    void getItemInNodeContact(Ref* sender, const std::string& message);

    void appearHandButton();

    void getLocaSpawn();

    void spawnPlayer(float x, float y);

    bool onContactBegin(PhysicsContact& contact);
    bool onContactSeparate(PhysicsContact& contact);

    void pauseGame();
    void createUiMenu();

    void createButtonHand();

    void goToHome(cocos2d::Ref* pSender);

    cocos2d::TMXTiledMap* _tilemap;


    CREATE_FUNC(GameScene);

private:

    float spawnX = 0;
    float spawnY = 0;

    Layer* menuButton = nullptr;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    Sprite* Home = nullptr;
    Sprite* Dot = nullptr;
    MenuItemImage* HandButton = nullptr;

    PhysicsWorld* world;
    void setPhysicWorld(PhysicsWorld* _world)
    {
        world = _world;
    }
};

#endif // __GAME_SCENE_H__
