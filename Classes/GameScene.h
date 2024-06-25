#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "MenuLayer.h"
#include "UICompleteGame.h"

using namespace cocos2d;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createPhysicsWorld();

    virtual bool init();

    void decreaseVolumnBackGroundMusic();

    int loadLevel();
    int loadHighScoreLevel();
    int level = 1;

    Player* player;

    bool createTileMap();
    void addBackGroundToTilemap();

    void getGround();
    void getWall();
    void getCeiling();
    void getBarrier();
    void getAllQuest();
    void getDoor();
    void getHideItem();
    void getLadder();
    void getTrap();
    void getSwitch();

    void getTrapByName(std::string trapName);
    //void setColorLayer();
    //Color3B colorFromHex(const std::string& hexString);

    void getQuestList();

    void visibleNextLevel(float dt);

    void turnTrap(Ref* sender, const std::string& controlObjects, const std::string& name, bool& status, Sprite* sprite);
    void useLadder(Ref* sender, const std::string& nameLadder, const std::string& location, float width, float height);

    void getItemInTileMapWithName(std::string name, bool visible);
    void getItemInNodeContact(Ref* sender, const std::string& message);
    void requestItemForNodeContact(Ref* sender, const std::string& request, const std::string& reward, Sprite* sprite);
    void visibleChatIcon(bool TF, std::string itemName);
    void completeTheQuest();
    void sortItemInventory();
    void playerShowItem(float dt);

    void appearHandButton();

    void getLocaSpawn();
    void getScaleSizeInTileMap();

    void spawnPlayer(float x, float y);

    bool onContactBegin(PhysicsContact& contact);
    bool onContactSeparate(PhysicsContact& contact);

    void playerMoveLR(float dt);

    void pauseGame();
    void createUiMenu();

    void createButtonHand();

    void goToHome(cocos2d::Ref* pSender);
    void goToSelectLevelMenu();

    cocos2d::TMXTiledMap* _tilemap;

    void winLevel(Ref* sender, int level);

    void CalculateNewSizeTile();

    CREATE_FUNC(GameScene);

private:
    bool stopFlag = false;
    float direction = 1;

    std::vector<Rect*> listOfLadder;
    //MenuItemSprite* buttonMove = nullptr;
    float scaleS = 1;
    float scaleSizeInMap = 1;

    float spawnX = 0;
    float spawnY = 0;

    //Layer* menuButton = nullptr;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    //Sprite* Home = nullptr;
    //Sprite* Dot = nullptr;
    //MenuItemImage* HandButton = nullptr;

    PhysicsWorld* world;
    void setPhysicWorld(PhysicsWorld* _world)
    {
        world = _world;
    }
};

#endif // __GAME_SCENE_H__