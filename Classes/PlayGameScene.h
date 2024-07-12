#ifndef __PLAYGAME_SCENE_H__
#define __PLAYGAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "tutorial.h"
#include "MenuLayer.h"
#include "MenuScene.h"

using namespace cocos2d;

class PlayGameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createPhysicsWorld();

    static cocos2d::Scene* createScene();

    virtual bool init();



    CREATE_FUNC(PlayGameScene);

private:
    PhysicsWorld* world;
    void setPhysicWorld(PhysicsWorld* _world)
    {
        world = _world;
    }

    int level = 1;
    void loadLevel();
    int scene = 1;

    // Khai báo mảng chứa các đối tượng TMXTiledMap*
    std::vector<cocos2d::TMXTiledMap*> tileMaps;

    //create tilemap
    //cocos2d::TMXTiledMap* _tilemap;
    bool addTileMapToTileMaps();
    bool createTileMap(cocos2d::TMXTiledMap* _tilemap, int _scene);
    void changeScene(int _scene);

    //test
    //std::vector<cocos2d::TMXTiledMap*> _tilemaps;
    void testCreateTileMap();

    Size tileSize;
    std::vector<float> speedList;
    float speed = 1;

    void getSpawnPoint(cocos2d::TMXTiledMap* _tilemap);
    std::vector<Player*> playerList;
    Player* player;

    std::vector<std::string> items;

    float spawnX = 0;
    float spawnY = 0;

    float moveToX = 0;
    float moveToY = 0;
    void spawnPlayer(cocos2d::TMXTiledMap* _tilemap);
    void playerMoveTo(float x);

    void getListTutorial();//


    void getGround(cocos2d::TMXTiledMap* _tilemap);
    void getHiddenPassageAndAddSprite(cocos2d::TMXTiledMap* _tilemap);
    void getItem(cocos2d::TMXTiledMap* _tilemap);
    void setVisibleButtonNode(std::string name, int ID, bool TF);
    void getBarrier(cocos2d::TMXTiledMap* _tilemap);

    //next (level and scene)
    void getNextInTilemap(cocos2d::TMXTiledMap* _tilemap);
    void next();
    void nextButtonHandle(Ref* sender);

    //level
    int loadHighScoreLevel();

    //tutorial
    void getTutorial(cocos2d::TMXTiledMap* _tilemap);
    bool tutorial = false;
    int tutorialMax = 0;
    int tutorialNum = 0;
    void visibleTutorialNum(float dt);
    void completeTutorialNum(int number, float delay);
    ////////
    //chua xong
    //notification
    void getNotification(cocos2d::TMXTiledMap* _tilemap);//chua xong

    //suggest
    void getSuggest(cocos2d::TMXTiledMap* _tilemap);//chua xong

    //gate
    void getGateBarrier(cocos2d::TMXTiledMap* _tilemap); //chua xong

    //puzzle
    void getPuzzle(cocos2d::TMXTiledMap* _tilemap);//chua xong

    //gate
    void getGate(cocos2d::TMXTiledMap* _tilemap);

    //change scene
    void getNextScene(cocos2d::TMXTiledMap* _tilemap);
    void getReturnScene(cocos2d::TMXTiledMap* _tilemap);

    //chua xong
    /////////
    //on-off  open-close door,trap,...
    void openHiddenPassage(std::string group);

    //set button for item, quest, switch...
    void itemButtonHandle(Ref* sender, Sprite* sprite);


    //event listener
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    bool onContactBegin(PhysicsContact& contact);
    bool onContactSeparate(PhysicsContact& contact);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

};

#endif // __HELLOWORLD_SCENE_H__
