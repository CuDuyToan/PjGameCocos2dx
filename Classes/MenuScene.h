#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

class MenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();


    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuOptionsCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);
    void buttonHandle(cocos2d::Ref* pSender);

    void increaseBackgroundMusicVolume(float volumeDelta);

    void createButton();
    void createBackGround(const std::string& backgroundPath);
    void mainBackGround();
    void playBackGroundMusic();
    void createGuraFallingSprite();
    void createAhhSharkSprite();
    void createImageGura();

    bool isBackgroundMusicPlaying();
    void saveMusicID();

    int musicID;

    CREATE_FUNC(MenuScene);

private:
    float scaleS = 1;
};

#endif // __MENU_SCENE_H__
