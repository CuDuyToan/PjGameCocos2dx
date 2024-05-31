#ifndef __LEVEL_SELECT_H__
#define __LEVEL_SELECT_H__

#include "cocos2d.h"

using namespace cocos2d;

class LevelSelectScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // callback khi người chơi chọn một cấp độ cụ thể
    void levelSelectedCallback(cocos2d::Ref* pSender);
    void homeButton(cocos2d::Ref* pSender);

    void menuButton();
    void createCasualButton();
    void createLevelText();
    void mainBackGround();

    void createButtonPageLevel1();

    void createButtonChangeLevel(const std::string& nameLevel);
    void createButtonPageLevel2();
    void createButtonPageLevel3();

    void saveNextLevelButtonState(bool state);

    bool loadNextLevelButtonState();

    void saveLevel(int level);

    int loadLevel();

    void createButtonPageLevel(int level);

    CREATE_FUNC(LevelSelectScene);

private:
    int a = 2;
};


#endif // __LEVEL_SELECT_H__